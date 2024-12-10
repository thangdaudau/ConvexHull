// Pure3DHullAlgos template header
#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <chrono>
#include <cassert>

#include "point3D.h"

namespace Core {

    // A face contains indices of its three point: a, b, c
    // and a normal vector: n
    template<typename T>
    struct Tface {
        int a, b, c;
        point3D<T> n;

        Tface(T _a, T _b, T _c, point3D<T> _n) : a(_a), b(_b), c(_c), n(_n) {
        }
    };

    /*
    * Warning: may modify the input!!!
    *
    * How to use:
    * auto hull = ConvexHullMachine< * type of point * >::incrementalFast( * vector of point * );
    * 'hull' contains all faces of the convex hull and its normal vector.
    */
    template<typename T, T initialEpsilon = static_cast<T>(1e-9)>
    class ConvexHullMachine {
    public:
        using point_t = point3D<T>;
        using face_t = Tface<T>;

        static std::vector<face_t> bruteForce(std::vector<point_t>& p) {
            return bruteForceImplement(p);
        }

        static std::vector<face_t> giftWrapping(std::vector<point_t>& p) {
            return giftWrappingImplement(p);
        }

        static std::vector<face_t> incremental(std::vector<point_t>& p) {
            return incrementalImplement(p);
        }

        static std::vector<face_t> incrementalFast(std::vector<point_t>& p) {
            return incrementalFastImplement(p);
        }



    private:
        constexpr static T EPSILON = initialEpsilon;

        static bool isZero(const point_t& a) {
            if (std::abs(a.x) > EPSILON) {
                return false;
            }
            if (std::abs(a.y) > EPSILON) {
                return false;
            }
            if (std::abs(a.z) > EPSILON) {
                return false;
            }
            return true;
        }

        static point_t normalVector(const point_t& a, const point_t& b, const point_t& c) {
            return cross(b - a, c - a);
        }

        // O(n^2) cause i'm too lazy
        static void initialFace(std::vector<point_t>& p) {
            std::sort(p.begin(), p.end());
            p.erase(std::unique(p.begin(), p.end()), p.end());

            int n = int(p.size());
            assert(n > 3);

            // Find an edge that must be in convex hull
            // Something like 2D hull
            for (int i = 2; i < n; i++) {
                auto v = p[1] - p[0];
                auto nv = p[i] - p[1];
                if (cross(v, nv).z > EPSILON) {
                    std::swap(p[i], p[1]);
                }
            }

            // Find a face
            // Just brute force
            for (int i = 2; i < n; i++) {
                std::swap(p[i], p[2]);
                auto v = normalVector(p[0], p[1], p[2]);
                if (isZero(v)) {
                    continue;
                }
                if (v.x > 0) {
                    v = -v;
                }
                bool ok = true;
                for (int j = 3; j < n; j++) {
                    auto d = dot(p[j] - p[0], v);
                    if (d > EPSILON) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    break;
                }
            }
            if (normalVector(p[0], p[1], p[2]).x > EPSILON) {
                std::swap(p[1], p[0]);
            }
            for (int i = 3; i < n; i++) {
                if (std::abs(dot(p[i] - p[0], normalVector(p[0], p[1], p[2]))) > EPSILON) {
                    std::swap(p[i], p[3]);
                    break;
                }
            }
            assert("All points are coplanar" && std::abs(dot(p[3] - p[0], normalVector(p[0], p[1], p[2]))) > EPSILON);
        }

        static void initialTetrahedron(std::vector<point_t>& p) {
            int n = int(p.size());
            assert(n > 3);
            // Make sure that first 4 points are not coplanar
            for (int i = 1; i < n; i++) {
                if (p[i] != p[0]) {
                    std::swap(p[i], p[1]);
                    break;
                }
            }
            for (int i = 2; i < n; i++) {
                if (!isZero(normalVector(p[0], p[1], p[i]))) {
                    std::swap(p[i], p[2]);
                    break;
                }
            }
            for (int i = 3; i < n; i++) {
                if (std::abs(dot(p[i] - p[0], normalVector(p[0], p[1], p[2]))) > EPSILON) {
                    std::swap(p[i], p[2]);
                    break;
                }
            }
            assert("All points are coplanar" && std::abs(dot(p[3] - p[0], normalVector(p[0], p[1], p[2]))) > EPSILON);
        }

        static std::vector<face_t> bruteForceImplement(std::vector<point_t>& p) {
            // Duyệt qua tất cả bộ ba điểm (a, b, c)
            // kiểm tra xem có tồn tại hai điểm nằm khác phía với mặt đang xét không
            // nếu không thì mặt đó chắc chắn nằm trong convex hull

            // Làm sao để định hướng các mặt :)
            // Bfs?

            // TODO: nhường cho thằng Thăng
        }

        static std::vector<face_t> giftWrappingImplement(std::vector<point_t>& p) {
            initialFace(p);
            int n = int(p.size());

            std::vector<face_t> faces;
            std::vector<std::vector<bool>> edges(n, std::vector<bool>(n));

            auto addFace = [&](int a, int b, int c) {
                faces.emplace_back(a, b, c, cross(p[b] - p[a], p[c] - p[a]));
                edges[a][b] = edges[b][c] = edges[c][a] = true;
                };
            addFace(0, 1, 2);

            for (int i = 0; i < int(faces.size()); i++) {
                int x[4] = { faces[i].a, faces[i].b, faces[i].c, faces[i].a };
                auto& fn = faces[i].n;
                for (int k = 0; k < 3; k++) {
                    int a = x[k];
                    int b = x[k + 1];
                    if (!edges[b][a]) {
                        // Tricky part :)
                        auto ab = p[b] - p[a];
                        auto v = cross(ab, cross(ab, p[0] - p[b]));
                        int mnID = 0;
                        for (int j = 1; j < n; j++) {
                            auto q = cross(ab, p[j] - p[b]);
                            // If faces[i] and p[j] is coplanar 
                            // and p[j] is on the left of 'ab' when you are looking in the 'fn' direction
                            if (std::abs(dot(p[j] - p[b], fn)) < EPSILON && dot(q, fn) < -EPSILON) {
                                mnID = j;
                                break;
                            }
                            auto nv = cross(ab, q);
                            // If 'nv' is on the right of 'v' when you are looking from 'a' to 'b'
                            if (dot(cross(nv, v), ab) > EPSILON) {
                                v = nv;
                                mnID = j;
                            }
                        }
                        addFace(b, a, mnID);
                    }
                }
            }
            return faces;
        }

        static std::vector<face_t> incrementalImplement(std::vector<point_t>& p) {
            initialTetrahedron(p);
            int n = int(p.size());

            std::vector<face_t> faces;
            std::vector<std::vector<bool>> edges(n, std::vector<bool>(n));

            auto addFace = [&](int a, int b, int c) {
                faces.emplace_back(a, b, c, cross(p[b] - p[a], p[c] - p[a]));
                edges[a][b] = edges[b][c] = edges[c][a] = true;
                };
            addFace(0, 1, 2);
            addFace(0, 2, 1);

            for (int i = 3; i < n; i++) {
                faces.erase(std::remove_if(faces.begin(), faces.end(), [&](const auto& f) {
                    // If this face is visible to p[i], remove it
                    if (dot(p[i] - p[f.a], f.n) > EPSILON) {
                        edges[f.a][f.b] = edges[f.b][f.c] = edges[f.c][f.a] = false;
                        return true;
                    }
                    return false;
                    }), faces.end());

                int sz = int(faces.size());
                for (int j = 0; j < sz; j++) {
                    int x[4] = { faces[j].a, faces[j].b, faces[j].c, faces[j].a };
                    for (int k = 0; k < 3; k++) {
                        if (!edges[x[k + 1]][x[k]]) {
                            addFace(x[k + 1], x[k], i);
                        }
                    }
                }
            }
            return faces;
        }

        static std::vector<face_t> incrementalFastImplement(std::vector<point_t>& p) {
            initialTetrahedron(p);
            std::shuffle(p.begin() + 4, p.end(), std::mt19937_64((unsigned int)std::chrono::steady_clock::now().time_since_epoch().count()));
            int n = int(p.size());

            // Hash function for two integers
            auto h = [&](int a, int b) {
                return int64_t(a) * n + b;
                };

            std::vector<face_t> faces;
            std::vector<int> alive; // faces[i] alive untill alive[i]
            std::unordered_map<int64_t, int> edges;
            std::vector<std::vector<int>> Pconflict(n);
            std::vector<std::vector<int>> Fconflict;

            auto addFace = [&](int a, int b, int c) {
                int id = int(faces.size());
                alive.push_back(n);
                Fconflict.emplace_back(0);
                faces.emplace_back(a, b, c, cross(p[b] - p[a], p[c] - p[a]));
                edges[h(a, b)] = edges[h(b, c)] = edges[h(c, a)] = id;
                return id;
                };

            addFace(0, 1, 2);
            addFace(0, 2, 1);

            for (int j = 0; j < 2; j++) {
                for (int i = 3; i < n; i++) {
                    auto d = dot(p[i] - p[faces[j].a], faces[j].n);
                    if (d > EPSILON) {
                        Pconflict[i].push_back(j);
                    }
                    if (d >= -EPSILON) {
                        Fconflict[j].push_back(i);
                    }
                }
            }

            for (int i = 3; i < n; i++) {
                Pconflict[i].erase(std::remove_if(Pconflict[i].begin(), Pconflict[i].end(), [&](int fid) {
                    return alive[fid] == n ? !(alive[fid] = i) : true;
                    }), Pconflict[i].end());

                for (int fid : Pconflict[i]) {
                    int x[4] = { faces[fid].a, faces[fid].b, faces[fid].c, faces[fid].a };
                    for (int k = 0; k < 3; k++) {
                        int a = x[k];
                        int b = x[k + 1];
                        auto it = edges.find(h(b, a));
                        if (it == edges.end()) {
                            edges.erase(h(a, b));
                            continue;
                        }

                        int adjId = it->second;
                        if (alive[adjId] > i) {
                            int newFid = addFace(a, b, i);

                            // Merge two set of two adjacent faces
                            std::set_union(Fconflict[fid].begin(), Fconflict[fid].end(),
                                Fconflict[adjId].begin(), Fconflict[adjId].end(),
                                std::back_inserter(Fconflict[newFid]));

                            // Remove invisible point
                            Fconflict[newFid].erase(std::remove_if(Fconflict[newFid].begin(), Fconflict[newFid].end(), [&](int pid) {
                                return !(pid > i && dot(p[pid] - p[faces[newFid].a], faces[newFid].n) > EPSILON);
                                }), Fconflict[newFid].end());

                            // Inverse mapping
                            for (int pid : Fconflict[newFid]) {
                                Pconflict[pid].push_back(newFid);
                            }
                        }
                    }
                }
            }
            std::vector<face_t> ret;
            for (int i = 0; i < int(faces.size()); i++) {
                if (alive[i] == n) {
                    ret.push_back(std::move(faces[i]));
                }
            }
            return ret;
        }
    };



}