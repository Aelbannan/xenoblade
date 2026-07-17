The function compiles but its generated machine-code structure differs
materially from retail.

Correct the largest observable semantic discrepancy shown in the structural
report.

Prioritize:
1. Missing or extra calls
2. Missing or extra memory writes
3. Incorrect access widths or offsets
4. Incorrect return paths
5. Incorrect branch or loop structure
6. Incorrect constants

Do not optimize register allocation, instruction ordering, or exact MWCC
matching until structural alignment is reached.

Evidence supplied:
- Current source
- Current semantic summary
- Structural component scores
- Concrete missing/extra observations
- Retail decoded assembly
- Candidate decoded assembly
- Relevant declarations
- Parent lineage
- Previously attempted semantic changes

One-discrepancy rule: Identify one primary correction in your response.

```json
{
  "stage": "semantic_repair",
  "primary_discrepancy": "missing call to funcB on the nonzero state path",
  "source": "...",
  "expected_effect": {
    "calls": ["add funcB"],
    "cfg": ["preserve early return"]
  }
}
```
