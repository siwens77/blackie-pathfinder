# 🚗 Blackie Pathfinder

Blackie is a truck-driving dog who wants to plan the cheapest route between two cities!
Given a graph where each edge has two weights (`w1` and `w2`), help Blackie find a path from source **A** to destination **B** that minimizes the **Blackie-length**.

---

## 📖 Problem Description

You are given a directed or undirected graph `G` with `N` nodes and `M` edges.
Each edge `e` has two positive integer weights:

* `w1(e)` – the *distance* (affecting petrol cost)
* `w2(e)` – the *fee* (affecting highway tolls)

The **Blackie-length** of a path `P = (e₁, e₂, e₃, …)` is defined as:

```
Blackie-length(P) = A + B
where
A = Σ w1(eᵢ)  for all non-prime i
B = 3 × Σ w2(eᵢ)  for all prime i
```

Your task:
Find a path from node **A** to node **B** that minimizes this Blackie-length.

---

## 🧩 Input Format

```
N M
u₁ v₁ w1₁ w2₁
u₂ v₂ w1₂ w2₂
...
uₘ vₘ w1ₘ w2ₘ
```

* `N` — number of nodes (1 ≤ N ≤ 10⁵)
* `M` — number of edges (1 ≤ M ≤ 10⁶)
* `u`, `v` — endpoints of an edge
* `w1`, `w2` — edge weights (positive integers)

---

## 📤 Output Format

```
k
n₁ n₂ ... nₖ
```

* `k` — number of nodes on the found path
* `n₁ … nₖ` — node IDs in the path from source to destination

---

**Algorithm Explanation**
* here explanation to be uploaded...



---
