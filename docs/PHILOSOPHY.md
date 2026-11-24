# Engineering Philosophy

Calendar-World adopts the [Infiltrator Engineering Philosophy](https://github.com/Infiltrator-Projects/Libraries/blob/main/docs/PHILOSOPHY.md) as its governing standard. These are acceptance criteria, not a claim of completed qualification.

## Ownership and fidelity

This provider owns its calendar conversion model, including the documented epoch, leap rules, day boundary and chronology. Common owns the shared temporal ABI and discovery contract; applications own presentation. Do not duplicate provider algorithms in consumers or silently substitute another model. Identify historical evidence, conventions, approximations and unsupported variants.

## Correctness and qualification

Define input ranges, overflow behaviour, precision and reversibility where applicable. Validate independent reference cases, boundary transitions, invalid inputs and round-trip properties within the documented domain. Reject unsupported inputs explicitly. Keep dependencies, ABI compatibility and release identities auditable; qualify the exact distributed artifacts.

## Craft and documentation

Preserve postgraduate-level reasoning in comments and technical documentation: derivations, invariants, ownership, trade-offs and failure semantics. Follow the family's [visual design authority](https://github.com/Infiltrator-Projects/InfiltratorOS/blob/main/docs/PHILOSOPHY.md) without altering the meaning of the represented time or date. A passing build is not evidence of historical or numerical accuracy.
