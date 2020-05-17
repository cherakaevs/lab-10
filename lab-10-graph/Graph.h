#pragma once

#include <algorithm>
#include <limits>
#include <stdexcept>


template<typename TVertex, typename TEdge>
class Graph {

public:
	struct Edge {
		//TVertex srcVertex; // �����������
		TVertex dstVertex;
		TEdge edge;

		Edge(TVertex dstVertex, TEdge edge) : dstVertex(dstVertex), edge(edge) {}
	};

private:


	struct EdgeNode {
		Edge edge;
		EdgeNode* next;

		EdgeNode(Edge edge, EdgeNode* next) : edge(edge), next(next) {}
	};

	struct Vertex {
		TVertex vertex;
		EdgeNode* edges;

		Vertex() : edges(nullptr) {}

		explicit Vertex(TVertex vertex) : vertex(vertex), edges(nullptr) {}
	};

	Vertex* _graph;
	std::size_t _count;

	std::size_t GetVertexIndex(TVertex vertex) const {
		for (std::size_t i = 0; i < _count; ++i) {
			if (_graph[i].vertex == vertex) {
				return i;
			}
		}
		return _count;
	}

public:
	Graph() : _graph(nullptr), _count(0) {}

	Graph(const Graph&) = delete;

	Graph& operator=(const Graph&) = delete;

	~Graph() {
		delete[] _graph;
	}

	size_t GetVertexIndexOrThrow(TVertex vertex) const {
		const auto index = GetVertexIndex(vertex);
		if (index == _count)
			throw std::invalid_argument("Vertex not found!");
		return index;
	}

	[[nodiscard]] std::size_t GetVertexCount() const {
		return _count;
	}

	TVertex GetVertex(std::size_t index) const {
		return _graph[index].vertex;
	}

	bool hasThisEdge(TVertex srcVertex, TVertex dstVertex) {
		const auto srcIndex = GetVertexIndexOrThrow(srcVertex);
		const auto dstIndex = GetVertexIndexOrThrow(dstVertex);
		auto tmp = _graph[srcIndex].edges;
		if (!tmp)
			return false;
		if (tmp->edge.dstVertex == dstVertex) {
			return true;
		}
		while (tmp->next && tmp->next->edge.dstVertex != dstVertex) {
			tmp = tmp->next;
		}
		if (!tmp)
			return false;
		return true;
	}

	void AddVertex(TVertex vertex) {
		const auto index = GetVertexIndex(vertex);
		if (index != _count)
			throw std::invalid_argument("Vertex already exists!");
		auto graph = new Vertex[_count + 1];
		for (std::size_t i = 0; i < _count; ++i) {
			graph[i] = _graph[i];
		}
		graph[_count] = Vertex(vertex);
		delete[] _graph;
		_graph = graph;
		++_count;
	}

	

	void AddEdge(TVertex srcVertex, TVertex dstVertex, TEdge edge) {
		if (hasThisEdge(srcVertex, dstVertex)) return;
		const auto srcIndex = GetVertexIndexOrThrow(srcVertex);
		const auto dstIndex = GetVertexIndexOrThrow(dstVertex);
		Edge e(dstVertex, edge);
		_graph[srcIndex].edges = new EdgeNode(e, _graph[srcIndex].edges);
	}

	bool checkEdge(TVertex srcVertex, TVertex dstVertex) {
		const auto srcIndex = GetVertexIndexOrThrow(srcVertex);
		for (auto counter = 0; counter < GetEdgeCount(srcVertex); ++counter) {
			if (_graph[srcIndex].edges->edge.dstVertex == dstVertex)
				return true;
			_graph[srcIndex].edges = _graph[srcIndex].edges->next;
		}
		return false;
	}

	std::size_t GetEdgeCount(TVertex srcVertex) const {
		std::size_t count = 0;
		auto node = _graph[GetVertexIndexOrThrow(srcVertex)].edges;
		while (node) {
			++count;
			node = node->next;
		}
		return count;
	}

	Edge GetEdge(TVertex srcVertex, std::size_t index) const {
		auto node = _graph[GetVertexIndexOrThrow(srcVertex)].edges;
		for (size_t i = 0; i < index; ++i)
			node = node->next;
		return node->edge;
	}


	void removeVertex(TVertex vertex) {
		const auto srcIndex = GetVertexIndexOrThrow(vertex);

		auto tmp = _graph[srcIndex].edges;
		while (tmp) {
			tmp = tmp->next;
			delete _graph[srcIndex].edges;
			_graph[srcIndex].edges = tmp;
		}

		for (auto i = 0; i < _count; ++i) {
			if (i == srcIndex)
				continue;    
			auto tmp = _graph[i].edges;
			if (!tmp)
				continue;
			if (tmp->edge.dstVertex == vertex) {
				_graph[i].edges = _graph[i].edges->next;
				delete tmp;
				continue;
			}
			while (tmp->next) {
				if (tmp->next->edge.dstVertex == vertex) {
					auto after = tmp->next->next;
					delete tmp->next;
					tmp->next = after;
					break;
				}
				tmp = tmp->next;
			}
		}

		auto graph = new Vertex[_count - 1];

		for (auto i = 0; i < srcIndex; ++i)
			graph[i] = _graph[i];

		for (auto i = srcIndex + 1; i < _count; ++i)
			graph[i - 1] = _graph[i];

		delete _graph;
		_graph = graph;
		--_count;
	}


	void removeEdge(TVertex srcVertex, TVertex dstVertex) {
		
			const auto srcIndex = GetVertexIndexOrThrow(srcVertex);
			const auto dstIndex = GetVertexIndexOrThrow(dstVertex);
			auto tmp = _graph[srcIndex].edges;
			if (!tmp)
				throw std::invalid_argument("Edge not found");
			if (tmp->edge.dstVertex == dstVertex) {
				_graph[srcIndex].edges = _graph[srcIndex].edges->next;
				delete tmp;
				return;
			}
			while (tmp && tmp->next && tmp->next->edge.dstVertex != dstVertex) {
				tmp = tmp->next;
			}
			if (!tmp)
				throw std::invalid_argument("Edge not found");

			if (tmp->next != nullptr) {
				auto after = tmp->next->next;
				delete tmp->next;
				tmp->next = after;
			}
	}

	void changeVertex(TVertex chgVertex, TVertex newVertex) {
		auto index = GetVertexIndexOrThrow(chgVertex);
		_graph[index].vertex = newVertex;
	}

	void editEdge(TVertex srcVertex, TVertex dstVertex, TVertex newDstVertex) {
		auto tmp = rand() % 7000;
		removeEdge(srcVertex, dstVertex);
		AddEdge(srcVertex, newDstVertex, tmp);
	}

};

template<typename TVertex, typename TEdge, typename TFunctional>
void DepthFirstSearch(const Graph<TVertex, TEdge> &graph, TVertex begin, TFunctional f) {
    const auto n = graph.GetVertexCount();
    auto visited = new bool[n];
    auto stack = new std::size_t[n];
    std::size_t stackSize = 0;
    for (std::size_t i = 0; i < n; ++i) {
        visited[i] = false;
    }

    try {
        const auto beginIndex = graph.GetVertexIndexOrThrow(begin);
        stack[stackSize++] = beginIndex;
        visited[beginIndex] = true;

        while (stackSize > 0) {
            const auto vertexIndex = stack[--stackSize];
            const auto vertex = graph.GetVertex(vertexIndex);
            f(vertex);
            const auto edgeCount = graph.GetEdgeCount(vertex);
            for (std::size_t edgeIndex = 0; edgeIndex < edgeCount; ++edgeIndex) {
                const auto dstIndex = graph.GetVertexIndexOrThrow(graph.GetEdge(vertex, edgeIndex).dstVertex);
                if (!visited[dstIndex]) {
                    stack[stackSize++] = dstIndex;
                    visited[dstIndex] = true;
                }
            }

        }
    }
    catch (...) {
        delete[] visited;
        delete[] stack;
        throw;
    }
    delete[] visited;
    delete[] stack;
}

template<typename TVertex, typename TEdge, typename TFunctional>
void recDepthFirstSearch(const Graph<TVertex, TEdge>& graph, TVertex begin, bool* used, std::size_t* stack, std::size_t& stackSize,
	TFunctional f) {

	auto beginIndex = graph.FindVertexIndexOrThrow(begin);
	used[beginIndex] = true;
	f(graph.GetVertex(beginIndex));
	const TVertex vertex = graph.GetVertex(beginIndex);
	for (std::size_t j = 0; j < graph.getEdgeCount(vertex); ++j) {
		beginIndex = graph.FindVertexIndexOrThrow(graph.GetEdge(vertex, j).dstVertex);
		if (!used[beginIndex]) {
			stack[stackSize++] = beginIndex;
			used[beginIndex] = true;
		}
	}
	while (stackSize > 0) {
		auto vertexIndex = stack[--stackSize];
		recDepthFirstSearch(graph, graph.GetVertex(vertexIndex), used, stack, stackSize, f);
	}

}


template<typename TVertex, typename TEdge, typename TFunctional>
void BreadthFirstSearch(const Graph<TVertex, TEdge> &graph, TVertex begin, TFunctional f) {
    const auto n = graph.GetVertexCount();
    auto visited = new bool[n];
    auto queue = new std::size_t[n];
    std::size_t push = 0;
    std::size_t pop = 0;
    for (std::size_t i = 0; i < n; ++i) {
        visited[i] = false;
    }

    try {
        const auto beginIndex = graph.GetVertexIndexOrThrow(begin);
        queue[push++] = beginIndex;
        visited[beginIndex] = true;

        while (push != pop) {
            const auto vertexIndex = queue[pop++];
            const auto vertex = graph.GetVertex(vertexIndex);
            f(vertex);
            const auto edgeCount = graph.GetEdgeCount(vertex);
            for (std::size_t edgeIndex = 0; edgeIndex < edgeCount; ++edgeIndex) {
                const auto dstIndex = graph.GetVertexIndexOrThrow(graph.GetEdge(vertex, edgeIndex).dstVertex);
                if (!visited[dstIndex]) {
                    queue[push++] = dstIndex;
                    visited[dstIndex] = true;
                }
            }

        }
    }
    catch (...) {
        delete[] visited;
        delete[] queue;
        throw;
    }
    delete[] visited;
    delete[] queue;
}

template<typename TEdge>
struct DefaultEdgeWeight {
    float operator()(TEdge edge) {
        return static_cast<float>(edge);
    }
};

const auto INFINITY_DISTANCE = std::numeric_limits<float>::infinity();

template<typename TVertex, typename TEdge, typename TEdgeWeight = DefaultEdgeWeight<TEdge>>
float Dijkstra(const Graph<TVertex, TEdge> &graph,
               TVertex begin,
               TVertex end,
               TVertex *path,
               std::size_t *pathLength) {
    const auto n = graph.GetVertexCount();
    auto d = new float[n];
    auto u = new bool[n];
    auto p = new std::size_t[n];
    for (std::size_t i = 0; i < n; ++i) {
        d[i] = INFINITY_DISTANCE;
        u[i] = false;
        p[i] = n;
    }

    try {
        const auto beginIndex = graph.GetVertexIndexOrThrow(begin);
        d[beginIndex] = 0;

        for (std::size_t i = 0; i < n; ++i) {
            // ���� 1: ������� ������������ ������� � ������������ �����������
            std::size_t selectedIndex = n;
            float selectedDistance = INFINITY_DISTANCE;
            for (std::size_t j = 0; j < n; ++j) {
                if (!u[j] && d[j] < selectedDistance) {
                    selectedIndex = j;
                    selectedDistance = d[j];
                }
            }
            if (selectedIndex == n)
                break;
            u[selectedIndex] = true;

            // ���� 2: ��� ���� ������� ���� ��������� ������� �������� ����������
            const auto selectedVertex = graph.GetVertex(selectedIndex);
            const auto edgeCount = graph.GetEdgeCount(selectedVertex);
            for (std::size_t j = 0; j < edgeCount; ++j) {
                const auto edge = graph.GetEdge(selectedVertex, j);
                const auto dstVertex = edge.dstVertex;
                const auto dstIndex = graph.GetVertexIndexOrThrow(dstVertex);
                const auto distance = d[selectedIndex] + TEdgeWeight()(edge.edge);
                if (distance < d[dstIndex]) {
                    d[dstIndex] = distance;
                    p[dstIndex] = selectedIndex;
                }
            }
        }

        const auto endIndex = graph.GetVertexIndexOrThrow(end);
        const auto distance = d[endIndex];
        *pathLength = 0;
        if (distance != INFINITY_DISTANCE && path != nullptr) {
            auto currentIndex = endIndex;
            while (currentIndex != beginIndex) {
                path[(*pathLength)++] = graph.GetVertex(currentIndex);
                currentIndex = p[currentIndex];
            }
            path[(*pathLength)++] = begin;
            std::reverse(path, path + *pathLength);
        }
        delete[] d;
        delete[] u;
        delete[] p;
        return distance;
    }
    catch (...) {
        delete[] d;
        delete[] u;
        delete[] p;
        throw;
    }
}

template<typename TVertex, typename TEdge, typename TEdgeWeight = DefaultEdgeWeight<TEdge>>
float BellmanFord(const Graph<TVertex, TEdge> &graph,
                  TVertex begin,
                  TVertex end,
                  TVertex *path,
                  std::size_t *pathLength) {
    const auto n = graph.GetVertexCount();
    auto d = new float[n];
    auto p = new std::size_t[n];
    for (std::size_t i = 0; i < n; ++i) {
        d[i] = INFINITY_DISTANCE;
        p[i] = n;
    }

    try {
        const auto beginIndex = graph.GetVertexIndexOrThrow(begin);
        d[beginIndex] = 0;

        for (std::size_t i = 0; i < n - 1; ++i) {
            // ���������� ��� ����!
            // ��� ��� ���� ����������� ������� ���������, ��� ����� ��� �����:
            // ������� ���������� ��� �������, � ��� ��� ������ ������� ���������� ��� ����.
            for (std::size_t vertexIndex = 0; vertexIndex < n; ++vertexIndex) {
                const auto vertex = graph.GetVertex(vertexIndex);
                const auto edgeCount = graph.GetEdgeCount(vertex);
                for (std::size_t edgeIndex = 0; edgeIndex < edgeCount; ++edgeIndex) {
                    const auto edge = graph.GetEdge(vertex, edgeIndex);
                    const auto dstVertex = edge.dstVertex;
                    const auto dstIndex = graph.GetVertexIndexOrThrow(dstVertex);
                    const auto distance = d[vertexIndex] + TEdgeWeight()(edge.edge);
                    if (distance < d[dstIndex]) {
                        d[dstIndex] = distance;
                        p[dstIndex] = vertexIndex;

                    }
                }
            }
        }

        const auto endIndex = graph.GetVertexIndexOrThrow(end);
        const auto distance = d[endIndex];
        *pathLength = 0;
        if (distance != INFINITY_DISTANCE && path != nullptr) {
            auto currentIndex = endIndex;
            while (currentIndex != beginIndex) {
                path[(*pathLength)++] = graph.GetVertex(currentIndex);
                currentIndex = p[currentIndex];
            }
            path[(*pathLength)++] = begin;
            std::reverse(path, path + *pathLength);
        }
        delete[] d;
        delete[] p;
        return distance;
    }
    catch (...) {
        delete[] d;
        delete[] p;
        throw;
    }
}


