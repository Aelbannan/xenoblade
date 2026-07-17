# Pull-request and review requirements

## Required evidence for every semantic change

Any pull request changing decoder, semantics, stack logic, memory behavior, contracts, engine query construction, certificate interpretation, or promotion policy must include:

1. Architecture-model impact declaration.
2. Normative scope/documentation impact.
3. Positive tests.
4. At least one adversarial negative test.
5. ConcreteOps differential evidence where applicable.
6. Dolphin evidence for changed opcode behavior.
7. Mutation-test impact for critical logic.
8. Cache/certificate invalidation analysis.
9. Promotion-policy impact.
10. Reproduction artifacts for any historical bug fixed.

### Model bump policy

Bump the architecture model when a change can alter whether a proof is `EQUIVALENT`, `NOT_EQUIVALENT`, or inconclusive for the same input request. Examples:

- instruction semantics correction;
- observability change;
- stack masking change;
- relocation constraint change;
- FP domain change;
- call summary behavior change;
- validity/exception policy change.

A pure performance optimization with bit-for-bit identical queries and results need not bump the model, but should still alter engine-source provenance.

### Result-format bump policy

Bump result format when:

- required fields change;
- field meanings change;
- canonical serialization changes;
- downstream parsers need different behavior.

### Certificate-version bump policy

Bump certificate version when:

- signed payload changes;
- validation rules change;
- transitive-root algorithm changes;
- confidence/policy evidence becomes part of acceptance.
