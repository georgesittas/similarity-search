# Similarity Search

This project contains implementations for the following dimensionality reduction methods and their application to
the domains of similarity search (i.e. approximate k-Nearest Neighbors) and clustering:

- [Locality Sensitive Hashing (stable distributions)](https://en.wikipedia.org/wiki/Locality-sensitive_hashing)
- [Randomized Projections into Hamming Hypercubes](https://www.researchgate.net/publication/311842520_Practical_linear-space_Approximate_Near_Neighbors_in_high_dimension)

```
Clustering through reverse assignment using range queries
---------------------------------------------------------

1. Index all data points of interest in either data structure (LSH hash tables or a single Hypercube hash table)
2. Initialize all cluster centroids
3. At each iteration, for each centroid execute a ranged query around it and assign all neighboring points to it  
4. The initial radii can be computed as min(dist between centers)/2 and after each iteration they are doubled
5. Repeat the above steps until the algorithm converges to a fix point, or certain criteria are met
6. If there are any unassigned points, we can assign them using the classic K-Means algorithm
7. Output computed clusters
```

For clustering, we used the initialization scheme of [K-Means++](https://en.wikipedia.org/wiki/K-means%2B%2B) and
we implemented the classic [Lloyd's algorithm](https://en.wikipedia.org/wiki/K-means_clustering), comparing it to
the above methods based on the well-known [Silhouette](https://en.wikipedia.org/wiki/Silhouette_(clustering)) metric.
See the [project documentation, benchmark results](https://github.com/GeorgeSittas/Similarity-Search/blob/main/report.pdf)
and the related [bibliography](https://github.com/GeorgeSittas/Similarity-Search/blob/main/details.pdf).

## Contributors 

• [George Sittas (Jo)](https://github.com/GeorgeSittas)\
• [Dimitra Kousta (Demesta)](https://github.com/Demesta)
