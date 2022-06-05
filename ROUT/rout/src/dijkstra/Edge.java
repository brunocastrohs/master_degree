package dijkstra;

public class Edge {
	public final Node vizinho;
	public final double valorAresta;
	public int gid;

	public Edge(Node argTarget, double argWeight) {
		vizinho = argTarget;
		valorAresta = argWeight;
	}
	
	public Edge(int gid,Node argTarget, double argWeight) {
		vizinho = argTarget;
		valorAresta = argWeight;
		this.gid = gid;
	}
}