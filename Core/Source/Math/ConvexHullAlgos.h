#pragma once
#include <vector>
#include <set>
#include <numeric>
#include <algorithm>
#include <glm/glm.hpp>
#include "Math/Rng.h"
#include <cassert>

namespace Core {

	enum class ConvexHullAlgoType 
	{
		none = -1, GiftWrapping, Incremental
	};
	class ConvexHullAlgos
	{
	public:
		using edge_t = std::pair<int, int>;
		using face_t = std::tuple<int, int, int>;

		void reset(ConvexHullAlgoType type, int numberOfPoints);
		void restart();
		bool nextState();

		const std::vector<glm::vec3>& getPoints() const { return m_points; }
		const std::vector<face_t>& getFaces() const { return m_faces; }
		const std::vector<face_t>& getVisibleFaces() const { return m_visibleFaces; }
		const std::vector<unsigned int>& getPointOrder() const { return m_ord; }
		// Index of m_faces for GiftWrapping, m_points for Incremental
		int getCurrentIndex() const { return m_currentIndex; }
		int getRemainFaceCount() const { return m_remainFaceCount; }
		void clearVisibleFace() { m_visibleFaces.clear(); }
	private:
		// gift wrapping
		bool nextFace(); 
		void initialFace();
		bool ensureFace(int id);
		// incremental
		bool nextPoints(); 
		void initialTetrahedron();

		glm::vec3 normalVector(const face_t& face);
		void addFace(int a, int b, int c);
		void removeFace(const face_t& face);
		bool isZero(const glm::vec3& v);
		bool canSee(const face_t& face, const glm::vec3& point);
	private:
		ConvexHullAlgoType m_type = ConvexHullAlgoType::none;

		std::vector<glm::vec3> m_points;
		std::vector<unsigned int> m_ord;
		std::set<edge_t> m_edges;
		std::vector<face_t> m_faces;
		int m_currentIndex = 0;

		// incremental val
		std::vector<face_t> m_visibleFaces; 
		int m_remainFaceCount = 0;

	private:
		constexpr static float s_EPS = 1e-3f;
	};
}