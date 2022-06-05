package dijkstra;

public class Node implements Comparable<Node> {
	public String name;
	public int gid;
	public Edge[] adjacencies;
	public double minDistance = Double.POSITIVE_INFINITY;
	public Node previous = null;

	public Node(String argName) {
		name = argName;
	}

	public Node(String argName,int gid) {
		this.name = argName;
		this.gid = gid;
	}
	
	public String toString() {
		return name;
	}

	public int compareTo(Node other) {
		return Double.compare(minDistance, other.minDistance);
	}
	
	@Override
	public boolean equals(Object obj) {
		if(obj instanceof Node){
			Node n = (Node)obj;
		
			return this.gid == n.gid?true:false;
		}
		else
			return false;
	}
	
}
