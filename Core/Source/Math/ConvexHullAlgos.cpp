#include "ConvexHullAlgos.h"

namespace Core {

	
	void ConvexHullAlgos::reset(ConvexHullAlgoType type, int numberOfPoints)
	{
		m_currentIndex = 0;
		m_edges.clear();
		m_faces.clear();
		m_points.clear();
		m_ord.clear();
		m_points.resize(numberOfPoints);
		assert(numberOfPoints > 3 && "Number of points is too small!");
		for (glm::vec3& vec : m_points) {
			vec = glm::vec3(Rng::Randfloat(-5, 5),
							Rng::Randfloat(-5, 5),
							Rng::Randfloat(-5, 5));
		}
		m_ord.resize(numberOfPoints);
		std::iota(m_ord.begin(), m_ord.end(), 0);
		switch (m_type = type) 
		{
		case ConvexHullAlgoType::GiftWrapping: initialFace(); break;
		case ConvexHullAlgoType::Incremental: initialTetrahedron(); break;
		default: assert(false && "Type does not match any blah blah.");
		}
	}

	void ConvexHullAlgos::restart()
	{
		m_faces.clear();
		m_edges.clear();
		m_currentIndex = 0;
	}

	bool ConvexHullAlgos::nextState()
	{
		return m_type == ConvexHullAlgoType::GiftWrapping ? nextFace() : nextPoints();
	}

	bool ConvexHullAlgos::nextFace()
	{
		if (m_currentIndex == (int)m_faces.size()) {
			return false;
		}
		glm::vec3 norm;
		auto wrap = [&norm, this](int a, int b)
		{
			if (m_edges.find({b, a}) != m_edges.end()) 
				return;
			glm::vec3 ab = m_points[b] - m_points[a];
			int id = -1;
			glm::vec3 h;
			for (int i : m_ord)
			{
				if (isZero(normalVector(face_t(a, b, i))))
				{
					continue;
				}
				if (std::abs(glm::dot(norm, m_points[i] - m_points[a])) <= s_EPS)
				{
					if (glm::dot(normalVector(face_t(a, b, i)), norm) < -s_EPS)
					{
						id = i;
						break;
					}
				}
				glm::vec3 nh = glm::cross(ab, normalVector(face_t(a, b, i)));
				if (id == -1 or glm::dot(ab, glm::cross(nh, h)) > s_EPS)
				{
					id = i;
					h = nh;
				}
			}
			assert(id != -1);
			addFace(b, a, id);
#ifdef DEBUG
			//assert(ensureFace((int)m_faces.size() - 1) && "Oh no, the algorithm is broke :(");
			if (not ensureFace(int(m_faces.size()) - 1))
			{
				printf("Oops!\n");
			}
#endif // DEBUG

		};
		auto [a, b, c] = m_faces[m_currentIndex++];
		norm = normalVector(face_t(a, b, c));
		wrap(a, b);
		wrap(b, c);
		wrap(c, a);
		return true;
	}

	bool ConvexHullAlgos::nextPoints()
	{
		m_visibleFaces.clear();
		if (m_currentIndex == (int)m_points.size())
		{
			m_remainFaceCount = (int)m_faces.size();
			return false;
		}
		int i = m_ord[m_currentIndex++];
		glm::vec3 p = m_points[i];
		m_faces.erase(std::remove_if(m_faces.begin(), m_faces.end(),
			[&p, this](const face_t& face) {
				if (canSee(face, p))
				{
					removeFace(face);
					return true;
				}
				return false;
			}
		), m_faces.end());
		m_remainFaceCount = (int)m_faces.size();
		for (int j = 0; j < m_remainFaceCount; j++)
		{
			auto [a, b, c] = m_faces[j];
			if (m_edges.find({ b, a }) == m_edges.end())
				addFace(b, a, i);
			if (m_edges.find({ c, b }) == m_edges.end())
				addFace(c, b, i);
			if (m_edges.find({ a, c }) == m_edges.end())
				addFace(a, c, i);
		}
		return true;
	}

	void ConvexHullAlgos::initialFace()
	{
		int n = (int)m_points.size();
		
		std::sort(m_ord.begin(), m_ord.end(),
			[this](int i, int j) {
				return std::tie(m_points[i].z, m_points[i].x, m_points[i].y) < std::tie(m_points[j].z, m_points[j].x, m_points[j].y);
			}
		);

		glm::vec3 p0 = m_points[m_ord[0]];
		glm::vec3 p1 = m_points[m_ord[1]];
		for (int i = 2; i < n; i++)
		{
			int j = m_ord[i];
			if (glm::cross(p1 - p0, m_points[j] - p1).y > s_EPS)
			{
				p1 = m_points[j];
				std::swap(m_ord[i], m_ord[1]);
			}
		}

		glm::vec3 v01 = p1 - p0;
		glm::vec3 h = glm::cross(v01, normalVector(face_t(m_ord[0], m_ord[1], m_ord[2])));
		for (int i = 3; i < n; i++) {
			glm::vec3 new_h = glm::cross(v01, normalVector(face_t(m_ord[0], m_ord[1], m_ord[i])));
			if (isZero(new_h))
				continue;
			float dir = glm::dot(v01, glm::cross(new_h, h));
			if (std::abs(dir) <= s_EPS)
				break;
			if (dir > s_EPS) 
			{
				h = new_h;
				std::swap(m_ord[i], m_ord[2]);
			}
		}
		assert(not isZero(h));
		if (normalVector(face_t(m_ord[0], m_ord[1], m_ord[2])).z > s_EPS)
			std::swap(m_ord[1], m_ord[0]);
		for (const auto& p : m_points)
		{
			float d = glm::dot(p - m_points[m_ord[0]], normalVector(face_t(m_ord[0], m_ord[1], m_ord[2])));
			if (d > s_EPS)
			{
				std::swap(m_ord[1], m_ord[2]);
				break;
			}
		}
		addFace(m_ord[0], m_ord[1], m_ord[2]);
#ifdef DEBUG
		assert(ensureFace(0) && "Initial face is wrong :((");
#endif // DEBUG
	}

	bool ConvexHullAlgos::ensureFace(int id)
	{
		const auto& f = m_faces[id];
		for (const auto& p : m_points) {
			if (glm::dot(p - m_points[std::get<0>(f)], normalVector(f)) > s_EPS)
			{
				printf("%f\n", glm::dot(p - m_points[std::get<0>(f)], normalVector(f)));
				return false;
			}
		}
		return true;
	}

	void ConvexHullAlgos::initialTetrahedron()
	{
		int n = (int)m_points.size();

		glm::vec3 p0 = m_points[m_ord[0]];
		for (int i = 1; i < n; i++) 
		{
			int j = m_ord[i];
			if (isZero(m_points[j] - m_points[0])) 
			{
				std::swap(m_ord[i], m_ord[1]);
				break;
			}
		}
		glm::vec3 p1 = m_points[m_ord[1]];
		for (int i = 2; i < n; i++) 
		{
			int j = m_ord[i];
			if (not isZero(glm::cross(m_points[j] - p0, p1 - p0)))
			{
				std::swap(m_ord[i], m_ord[2]);
			}
		}
		glm::vec3 p2 = m_points[2];
		auto normal_vector = glm::cross(p2 - p0, p1 - p0);
		for (int i = 3; i < n; i++) 
		{
			int j = m_ord[i];
			if (not isZero(glm::cross(normal_vector, m_points[j] - p0)))
			{
				std::swap(m_ord[3], m_ord[i]);
				break;
			}
		}
		addFace(m_ord[0], m_ord[1], m_ord[2]);
		addFace(m_ord[1], m_ord[0], m_ord[2]);
		m_remainFaceCount = 2;
		m_currentIndex = 3;
	}

	void ConvexHullAlgos::addFace(int a, int b, int c)
	{
		m_faces.emplace_back(a, b, c);
		m_edges.insert({ a, b });
		m_edges.insert({ b, c });
		m_edges.insert({ c, a });
	}

	void ConvexHullAlgos::removeFace(const face_t& face)
	{
		const auto& [a, b, c] = face;
		m_visibleFaces.push_back(face);
		m_edges.erase({ a, b });
		m_edges.erase({ b, c });
		m_edges.erase({ c, a });
	}

	glm::vec3 ConvexHullAlgos::normalVector(const face_t& face)
	{
		const auto& [a, b, c] = face;
		return glm::cross(m_points[b] - m_points[a], m_points[c] - m_points[b]);
	}

	bool ConvexHullAlgos::isZero(const glm::vec3& v)
	{
		if (std::abs(v.x) > s_EPS) {
			return false;
		}
		if (std::abs(v.y) > s_EPS) {
			return false;
		}
		if (std::abs(v.z) > s_EPS) {
			return false;
		}
		return true;
	}

	bool ConvexHullAlgos::canSee(const face_t& face, const glm::vec3& point)
	{
		return glm::dot(point - m_points[std::get<0>(face)], normalVector(face)) > s_EPS;
	}

}