//
// Created by scott on 11/17/25.
//
#include <cmath>
#include <iostream>
#include <vector>
#include <memory>


struct Vect3 {
    double x;
    double y;
    double z;

    Vect3(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Atom {
    int Z;          // atom number
    double mass;    // atomic mass
    Vect3 position;
    Vect3 velocity;
    Vect3 force;

    Atom(int Z,
        double mass,
        const Vect3 &position,
        const Vect3 &velocity,
        const Vect3 &force) :
        Z(Z), mass(mass), position(position), velocity(velocity), force(force) {}
};

struct System {
    std::vector<Atom> atoms;
};

void zero_forces(System &system) {
    for (auto& a : system.atoms) {
        a.force.x = 0.0;
        a.force.y = 0.0;
        a.force.z = 0.0;
    }
}

double compute_forces_lj



double lj_potential(double r, double epsilion = 1.0, double sigma = 17.8) {


    return 4.0 * epsilion * (sr12 - sr6);
}

double calculate_distance(Atom &a, Atom &b) {
    if (a.Z == b.Z) {
        // should not be here
    }
    double dx = a.position.x - b.position.x;
    double dy = a.position.y - b.position.y;
    double dz = a.position.z - b.position.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

void compute_forces_lj(System &system, double epsilon, double sigma, double r_cut = 0.0) {
    zero_forces(system);

    double sigma2 = sigma * sigma;
    double r_cut2 = (r_cut > 0.0 ? r_cut * r_cut : 0.0);
    double potential_energy = 0.0;

    //double inv_r2 = 1.0 / r;
    // double sr2 = sigma2 * inv_r2;
    // double sr6 = sr2 * sr2 * sr2;
    // double sr12 = sr6 * sr6;
    //
    double potential_energy = 0.0;
    const std::size_t n = system.atoms.size();

    for (std::size_t i ; i < n; ++i) {
        for (std::size_t j = i + 1; j<n; ++j) {
            double d = calculate_distance(system.atoms[j], system.atoms[i]);
            double v_ij = lj_potential(d);

            potential_energy += v_ij;

            double f_scalar = 24.0 * epsilon * (2.0 * sr12 - sr6)

            double fx = f
        }
    }
}


// half step velocities and update positions
void first_half_step(System &system, double dt) {
    for (auto& a : system.atoms) {
        double invMass = 1.0 / a.mass;
        a.velocity.x += 0.5 * dt * a.force.x * invMass;
        a.velocity.y += 0.5 * dt * a.force.y * invMass;
        a.velocity.z += 0.5 * dt * a.force.z * invMass;
    }
}



void second_half_step(System &system, double dt) {
    for (auto& a : system.atoms) {
        double invMass = 1.0 / a.mass;
        a.velocity.x += 0.5 * dt * a.force.x * invMass;
        a.velocity.y += 0.5 * dt * a.force.y * invMass;
        a.velocity.z += 0.5 * dt * a.force.z * invMass;
    }
}

void velocity_verlet(System &system, double dt) {
    // half step velocities and update positions
    first_half_step(system, dt);

    // forces will be updated by quantum engine
    compute_forces(system);

    // second half-step on velocities (after new forces)
    second_half_step(system, dt);
}

int main() {
    std::cout << "Starting Minimal MD" << std::endl;
    auto system = std::make_unique<System>();

    double dt = 0.001;
    double epsilon = 1.0;
    double sigma = 1.0;
    double r_cut = 2.5 * sigma;

    int n_steps = 10;

    double kinetic_energy = 0.0;
    double potential_energy = 0.0;

    Atom atom(1, 1, Vect3(0,0,0), Vect3(0,0,0), Vect3(0,0,0));
    system->atoms.emplace_back(atom);

    Atom atom2(2, 1, Vect3(1,1,1), Vect3(0,0,0), Vect3(0,0,0));
    system->atoms.emplace_back(atom2);

    std::cout << "Initial distance: " << calculate_distance(atom, atom2) << std::endl;

    for (int step = 0; step < n_steps; ++step) {
        std::cout << "Step " << step << std::endl;

        // 1 compute forces at current positions
        potential_energy = compute_forces_lj(system, epsilon, sigma, r_cut);
    }
}