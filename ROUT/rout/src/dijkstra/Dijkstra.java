package dijkstra;

import java.util.Map;
import java.util.PriorityQueue;
import java.util.List;
import java.util.ArrayList;
import java.util.Collections;

public class Dijkstra {
	
	int[] gid_sopt;
	
	public static Double cost = Double.POSITIVE_INFINITY;
	
	public static ArrayList<Node> computePath(Node source,Node target) {
		// cada vertice tem as suas arestas. o objeto aresta aqui possui o vetice adjacente e o custo.

		source.minDistance = 0.;
		// lista que organiza seus elementos baseado em um criterio no momento
		// da adi��o
		PriorityQueue<Node> nodeQueue = new PriorityQueue<Node>();
		// adciona o ponto inicial
		nodeQueue.add(source);

		while (!nodeQueue.isEmpty()) {
			// Tira de vertexQueue source e passa para u
			Node u = nodeQueue.poll();

			// itera arestas partindo de source
			for (Edge arestaDeU : u.adjacencies) {
				// vertice adjacente da aresta partindo de um dado vertice
				if (arestaDeU != null) {
					Node v = arestaDeU.vizinho;
					// peso de u at� v
					double valor = arestaDeU.valorAresta;
					// min distance
					double distanceThroughU = u.minDistance + valor;
					// seta qual o adjacente mais pr�ximo de u
					if (v != null && distanceThroughU < v.minDistance) {

						nodeQueue.remove(v);

						v.minDistance = distanceThroughU;

						v.previous = u;

						nodeQueue.add(v);

					}
				}
			}
			
			if(u.equals(target))
				break;
		}
		
		ArrayList<Node> path = new ArrayList<Node>();
		
		for (Node vertex = target; vertex != null; vertex = vertex.previous)
			path.add(vertex);
		
		Collections.reverse(path);
		
		return path;
		
	}

	public static ArrayList<Node> buildPath(Map<String, Node> graphManager) {

		System.out.print("Distance to " + graphManager.get("last") + " | ");
		
		ArrayList<Node> path = computePath(graphManager.get("start"),graphManager.get("penult"));
		
		if(!path.get(path.size()-1).equals((graphManager.get("last")))){
			
			path.add(graphManager.get("last"));
		
		//	computePath(graphManager.get("penult"),graphManager.get("last"));
			
			System.out.println("Path: " + path + " | Cost:"+ (graphManager.get("penult").minDistance));
			
			cost = graphManager.get("penult").minDistance;
		}
		else{	
			cost = graphManager.get("penult").minDistance;
			System.out.println("Path: " + path + " | Cost:"+ (graphManager.get("penult").minDistance));
		}
		return path;

	}

	public static void main(String[] args) {

		Node[] nodes = { new Node("Vertex 1"), new Node("Vertex 2"),
				new Node("Vertex 3"), new Node("Vertex 4"),
				new Node("Vertex 5"), new Node("Vertex 6") };

		nodes[0].adjacencies = new Edge[] { new Edge(nodes[1], 5),
				new Edge(nodes[2], 5) };
		nodes[1].adjacencies = new Edge[] { new Edge(nodes[0], 5),
				new Edge(nodes[3], 10) };
		nodes[2].adjacencies = new Edge[] { new Edge(nodes[0], 5),
				new Edge(nodes[4], 12) };
		nodes[3].adjacencies = new Edge[] { new Edge(nodes[1], 10),
				new Edge(nodes[5], 6) };
		nodes[4].adjacencies = new Edge[] { new Edge(nodes[2], 12),
				new Edge(nodes[5], 2) };
		nodes[5].adjacencies = new Edge[] { new Edge(nodes[3], 6),
				new Edge(nodes[4], 2) };

		System.out.print("Distance to " + nodes[5] + " | ");
		List<Node> path = computePath(nodes[0],nodes[5]);
		System.out.println("Path: " + path + " | Cost:" + nodes[5].minDistance);
		

	}
}