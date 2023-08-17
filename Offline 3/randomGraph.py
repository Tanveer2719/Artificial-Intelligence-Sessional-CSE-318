import random

def generate_graph(nodes, edges):
    if edges > nodes * (nodes - 1) // 2:
        raise ValueError("Too many edges for the given number of nodes.")

    graph = []
    for _ in range(edges):
        start_vertex = random.randint(1, nodes)
        end_vertex = random.randint(1, nodes)
        while end_vertex == start_vertex:
            end_vertex = random.randint(1, nodes)
        weight = random.randint(1, 100)
        graph.append((start_vertex, end_vertex, weight))

    return graph

def write_graph_to_file(graph, filename):
    with open(filename, 'w') as f:
        f.write(f"{len(graph)}\n")
        for edge in graph:
            start_vertex, end_vertex, weight = edge
            f.write(f"{start_vertex} {end_vertex} {weight}\n")

if __name__ == '__main__':
    num_nodes = 800
    num_edges = 1000

    random_graph = generate_graph(num_nodes, num_edges)
    write_graph_to_file(random_graph, 'random_graph.txt')
