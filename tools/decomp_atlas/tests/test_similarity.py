from __future__ import annotations

import unittest

from tools.decomp_atlas.lib.similarity import (
    compute_neighbors,
    cosine_similarity,
    hash_layout_2d,
    normalize_vector,
    project_2d,
    structural_vector,
)


class SimilarityTests(unittest.TestCase):
    def test_cosine_and_normalize(self) -> None:
        a = normalize_vector([1.0, 0.0, 0.0])
        b = normalize_vector([1.0, 0.0, 0.0])
        c = normalize_vector([0.0, 1.0, 0.0])
        self.assertAlmostEqual(cosine_similarity(a, b), 1.0)
        self.assertAlmostEqual(cosine_similarity(a, c), 0.0)

    def test_structural_vector_length(self) -> None:
        vec = structural_vector(
            size=64,
            direct_call_count=2,
            unresolved_call_count=1,
            has_indirect_calls=True,
            instruction_count=16,
            branch_count=3,
        )
        self.assertEqual(len(vec), 17)
        self.assertTrue(any(v != 0 for v in vec))

    def test_neighbors_and_projection(self) -> None:
        vectors = {
            "a": structural_vector(size=16, instruction_count=4, branch_count=1),
            "b": structural_vector(size=16, instruction_count=4, branch_count=1),
            "c": structural_vector(size=256, instruction_count=64, branch_count=20, has_indirect_calls=True),
        }
        neighbors = compute_neighbors(vectors, top_k=2)
        self.assertTrue(any(n["target_id"] == "a" and n["neighbor_id"] == "b" for n in neighbors))
        points = project_2d(vectors)
        self.assertEqual(set(points), {"a", "b", "c"})
        hx, hy = hash_layout_2d("missing")
        self.assertNotEqual((hx, hy), (0.0, 0.0))


if __name__ == "__main__":
    unittest.main()
