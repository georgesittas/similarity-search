# Similarity Search

This project contains our implementations for the following methods that solve the (approximate) k-Nearest Neighbors problem:

- [Locality Sensitive Hashing (LSH)](https://en.wikipedia.org/wiki/Locality-sensitive_hashing)
- [Randomized Projections into Hamming Hypercubes](https://www.researchgate.net/publication/311842520_Practical_linear-space_Approximate_Near_Neighbors_in_high_dimension)

These methods can be used to search for neighbors in a given radius around a data point (range query), and thus they can be applied
to the domain of clustering, as shown in the following algorithm:

```
Clustering through reverse assignment using range queries
---------------------------------------------------------

1. Index all data points of interest in either data structure (LSH hash tables or a single Hypercube hash table)
2. Initialize all cluster centroids
3. At each iteration, for each centroid execute a ranged query around it and assign all neighboring points to it
4. The initial radii can be computed as min(dist between centers)/2 and after each iteration they are doubled
5. Repeat the above steps until the algorithm converges to a fix point, or certain criteria are met
6. If there are any unassigned points, we can assign them using the classic K-Means algorithm.
7. Output computed clusters
```

In our case, we used the initialization scheme of [K-Means++](https://en.wikipedia.org/wiki/K-means%2B%2B). Additionally, we
implemented the classic [Lloyd's algorithm](https://en.wikipedia.org/wiki/K-means_clustering), and we compared it to the above
method, using the well-known [Silhouette](https://en.wikipedia.org/wiki/Silhouette_(clustering)) metric. See [here](https://github.com/GeorgeSittas/Similarity-Search/blob/main/report.pdf) for the project's documentation and the related benchmark results.

## Contributors 

• [George Sittas (Jo)](https://github.com/GeorgeSittas)\
• [Dimitra Kousta (Demesta)](https://github.com/Demesta)
