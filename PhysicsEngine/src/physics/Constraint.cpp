#include "Constraint.h"

MatMN Constraint::GetInvM() const {
    MatMN invM(6, 6);
    invM.Zero();
    invM.rows[0][0] = a->InverseMass;
    invM.rows[1][1] = a->InverseMass;
    invM.rows[2][2] = a->inverseI;
    invM.rows[3][3] = b->InverseMass;
    invM.rows[4][4] = b->InverseMass;
    invM.rows[5][5] = b->inverseI;
    return invM;
}

VecN Constraint::GetVelocities() const {
    VecN V(6);
    V.Zero();
    V[0] = a->velocity.x;
    V[1] = a->velocity.y;
    V[2] = a->angularVelocity;
    V[3] = b->velocity.x;
    V[4] = b->velocity.y;
    V[5] = b->angularVelocity;
    return V;
}

JointConstraint::JointConstraint() : Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f) {
    cachedLambda.Zero();
}

JointConstraint::JointConstraint(Body* a, Body* b, const Vec2& anchorPoint) : Constraint(), jacobian(1, 6), cachedLambda(1), bias(0.0f) {
    this->a = a;
    this->b = b;
    this->aPoint = a->WorldToLocal(anchorPoint);
    this->bPoint = b->WorldToLocal(anchorPoint);
    cachedLambda.Zero();
}

void JointConstraint::PreSolve(const float dt) {
    const Vec2 pa = a->LocalToWorld(aPoint);
    const Vec2 pb = b->LocalToWorld(bPoint);

    const Vec2 ra = pa - a->position;
    const Vec2 rb = pb - b->position;

    jacobian.Zero();

    Vec2 J1 = (pa - pb) * 2.0;
    jacobian.rows[0][0] = J1.x;
    jacobian.rows[0][1] = J1.y;

    float J2 = ra.Cross(pa - pb) * 2.0;
    jacobian.rows[0][2] = J2;

    Vec2 J3 = (pb - pa) * 2.0;
    jacobian.rows[0][3] = J3.x;
    jacobian.rows[0][4] = J3.y;

    float J4 = rb.Cross(pb - pa) * 2.0;
    jacobian.rows[0][5] = J4;

    const MatMN Jt = jacobian.Transpose();
    VecN impulses = Jt * cachedLambda;

    a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1]));
    a->ApplyImpulseAngular(impulses[2]);                  
    b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4]));
    b->ApplyImpulseAngular(impulses[5]);                  

    const float beta = 0.05f;
    float C = (pb - pa).Dot(pb - pa);
    C = std::max(0.0f, C - 0.01f);
    bias = (beta / dt) * C;
}

void JointConstraint::Solve() {
    const VecN V = GetVelocities();
    const MatMN invM = GetInvM();
    const MatMN J = jacobian;
    const MatMN Jt = jacobian.Transpose();

    MatMN lhs = J * invM * Jt;  // A
    VecN rhs = J * V * -1.0f;   // b
    rhs[0] -= bias;
    VecN lambda = MatMN::SolveGaussSeidel(lhs, rhs);
    cachedLambda += lambda;

    VecN impulses = Jt * lambda;

    a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1])); // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4])); // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse
}

void JointConstraint::PostSolve() {
}

PenetrationConstraint::PenetrationConstraint() : Constraint(), jacobian(2, 6), cachedLambda(2), bias(0.0f), friction(0) {
    cachedLambda.Zero();
}

PenetrationConstraint::PenetrationConstraint(Body* a, Body* b, const Vec2& aCollisionPoint, const Vec2& bCollisionPoint, const Vec2& normal) : Constraint(), jacobian(2, 6), cachedLambda(2), bias(0.0f), friction(0) {
    this->a = a;
    this->b = b;
    this->aPoint = a->WorldToLocal(aCollisionPoint);
    this->bPoint = b->WorldToLocal(bCollisionPoint);
    this->normal = a->WorldToLocal(normal);
    cachedLambda.Zero();
}

void PenetrationConstraint::PreSolve(const float dt) {
    const Vec2 pa = a->LocalToWorld(aPoint);
    const Vec2 pb = b->LocalToWorld(bPoint);
    Vec2 n = (pb - pa);
    n.Normalize();

    const Vec2 ra = pa - a->position;
    const Vec2 rb = pb - b->position;

    jacobian.Zero();

    Vec2 J1 = -n;
    jacobian.rows[0][0] = J1.x;
    jacobian.rows[0][1] = J1.y; 

    float J2 = -ra.Cross(n);
    jacobian.rows[0][2] = J2;  

    Vec2 J3 = n;
    jacobian.rows[0][3] = J3.x; 
    jacobian.rows[0][4] = J3.y;

    float J4 = rb.Cross(n);
    jacobian.rows[0][5] = J4;  

    friction = std::min(a->friction, b->friction);
    if (friction > 0)
    {
        Vec2 t = n.Normal();
        t.Normalize();

        jacobian.rows[1][0] = -t.x;
        jacobian.rows[1][1] = -t.y;
        jacobian.rows[1][2] = -ra.Cross(t);

        jacobian.rows[1][3] =  t.x;
        jacobian.rows[1][4] =  t.y;
        jacobian.rows[1][5] =  rb.Cross(t);
    }

    const MatMN Jt = jacobian.Transpose();
    VecN impulses = Jt * cachedLambda;

    a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1])); // A linear impulse
    a->ApplyImpulseAngular(impulses[2]);                   // A angular impulse
    b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4])); // B linear impulse
    b->ApplyImpulseAngular(impulses[5]);                   // B angular impulse

    const float beta = 0.2f;
    float C = (pb - pa).Dot(-n);
    C = std::min(0.0f, C + 0.01f);

    Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
    float vrelDotNormal = (va - vb).Dot(n);

    float e = std::max(a->restitution, b->restitution);

    bias = (beta / dt) * C + (e * vrelDotNormal);
}

void PenetrationConstraint::Solve() {
    const VecN V = GetVelocities();
    const MatMN invM = GetInvM();

    const MatMN J = jacobian;
    const MatMN Jt = jacobian.Transpose();

    MatMN lhs = J * invM * Jt;  // A
    VecN rhs = J * V * -1.0f;   // b
    rhs[0] -= bias;
    VecN lambda = MatMN::SolveGaussSeidel(lhs, rhs);

    VecN oldLambda = cachedLambda;
    cachedLambda += lambda;
    cachedLambda[0] = (cachedLambda[0] < 0.0f) ? 0.0f : cachedLambda[0];

    if (friction > 0)
    {
        const float maxFriction = cachedLambda[0] * friction;
        cachedLambda[1] = cachedLambda[1] >  maxFriction ?  maxFriction : cachedLambda[1];
        cachedLambda[1] = cachedLambda[1] < -maxFriction ? -maxFriction : cachedLambda[1];
    }

    lambda = cachedLambda - oldLambda;
    VecN impulses = Jt * lambda;
    
    a->ApplyImpulseLinear(Vec2(impulses[0], impulses[1])); 
    a->ApplyImpulseAngular(impulses[2]);                   
    b->ApplyImpulseLinear(Vec2(impulses[3], impulses[4])); 
    b->ApplyImpulseAngular(impulses[5]);                  
}

void PenetrationConstraint::PostSolve() {
}
