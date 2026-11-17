from pathlib import Path
from math import sqrt, isclose
import pyhepmc

N_EVENTS = 1000
OUTPUT = Path("inputs/piminus_10GeV_1000.hepmc")

# Units: momentum and energy in GeV; positions in mm.
MASS = 0.13957039

# Match the momentum printed in your previous particle-gun log.
PZ = 10.0
ENERGY = sqrt(PZ * PZ + MASS * MASS)

# Same beam position and direction as before.
X, Y, Z = 0.0, 50.5, -5000.0

OUTPUT.parent.mkdir(parents=True, exist_ok=True)

with pyhepmc.open(str(OUTPUT), "w") as writer:
    for event_number in range(N_EVENTS):
        event = pyhepmc.GenEvent(
            pyhepmc.Units.GEV,
            pyhepmc.Units.MM
        )
        event.event_number = event_number

        vertex = pyhepmc.GenVertex(
            pyhepmc.FourVector(X, Y, Z, 0.0)
        )

        momentum = pyhepmc.FourVector(
            0.0, 0.0, PZ, ENERGY
        )

        # Status 4: incoming bookkeeping particle.
        # This is not an additional pion to transport.
        incoming = pyhepmc.GenParticle(momentum, -211, 4)
        incoming.generated_mass = MASS

        # Status 1: the pion that Geant4 will transport.
        pion = pyhepmc.GenParticle(momentum, -211, 1)
        pion.generated_mass = MASS

        vertex.add_particle_in(incoming)
        vertex.add_particle_out(pion)
        event.add_vertex(vertex)

        writer.write(event)

# Read back and validate every event.
count = 0

with pyhepmc.open(str(OUTPUT)) as reader:
    for event in reader:
        assert event.event_number == count
        assert event.momentum_unit == pyhepmc.Units.GEV
        assert event.length_unit == pyhepmc.Units.MM

        primaries = [
            p for p in event.particles if p.status == 1
        ]
        assert len(primaries) == 1

        pion = primaries[0]
        assert pion.pid == -211
        assert isclose(pion.momentum.px, 0.0, abs_tol=1e-10)
        assert isclose(pion.momentum.py, 0.0, abs_tol=1e-10)
        assert isclose(pion.momentum.pz, PZ, abs_tol=1e-10)
        assert isclose(pion.momentum.e, ENERGY, abs_tol=1e-10)

        vertex = pion.production_vertex
        assert vertex is not None
        assert len(pion.parents) == 1

        position = vertex.position
        assert isclose(position.x, X, abs_tol=1e-10)
        assert isclose(position.y, Y, abs_tol=1e-10)
        assert isclose(position.z, Z, abs_tol=1e-10)

        count += 1

assert count == N_EVENTS

print(f"Created and validated: {OUTPUT}")
print(f"Events: {count}")
print("Transportable pions per event: 1")
print(f"Position: ({X}, {Y}, {Z}) mm")
print(f"Momentum: (0, 0, {PZ}) GeV/c")
print(f"Total energy: {ENERGY:.9f} GeV")
