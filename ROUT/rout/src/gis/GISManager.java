package gis;

import java.io.OutputStream;
import java.io.PrintWriter;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import utils.DatabaseUtil;
import utils.Place;
import dijkstra.Dijkstra;
import dijkstra.Edge;
import dijkstra.Node;

public class GISManager {

	static public int n = 0;

	static public Place[] places = null;

	static public int[][] D = null;
	
	static public HashMap<String, Node> graphManager = null;
	
	static public Map<String, Node> edgeManager = null;
	
	static public Node start = null;
	
	static public int dijkstraIndex = 0;
	
	static public boolean searchForBridge = true;
	
	static public int indexManager = 1; 

	public static int[][] retrieveDGIS(int kume) throws Exception {

		String sql = "SELECT DISTANCE,GID_1,X,Y,GID_2 FROM D WHERE kume_1 ="+kume+" AND kume_2 ="+kume+"  order by gid_1,gid_2";

		ResultSet rs = DatabaseUtil.executeQuery(sql);

		int length = retrieveN(kume) + 1;

		int[][] d = new int[length][length];

		places = new Place[length];

		int j = 1;

		// convers�o des dist�ncia � p�ssivo de obje��es
		for (int i = 1; i <= n; i++) {
			while (rs.next()) {
				d[i][j] = (int) (rs.getDouble(1));
				// System.out.print(" "+rs.getDouble(1));
				j++;
				if (j > n)
					break;
			}
			places[i] = new Place(rs.getInt(2), rs.getDouble(3),rs.getDouble(4));
			j = 1;
			// System.out.print(" \n");
		}

		D = d;

		return d;

	}

	public static int retrieveN(int kume) throws Exception {

		String sql = "SELECT count(gid) FROM cliente_ativo_distinct WHERE kume ="+kume;

		ResultSet rs = DatabaseUtil.executeQuery(sql);

		n = 0;

		while (rs.next()) {

			n = rs.getInt(1);

		}

		return n;

	}

	public static Double buildEuclideanWay(Place[] gis_sopt,boolean makeDisjkstranWay) throws Exception {

		String sql = "";
		
		Double gis_cost = 0.0;

		resetPath();

		for (int i = 1; i < n; i++) {
			sql = "INSERT INTO EUCLIDEAN_SEQUENCE(INDICE,OBJETO_GID,OBJETO_NEXT,OBJETO_GEOM,OBJETO_NEXT_GEOM) VALUES("
					+ indexManager+","+
					+ gis_sopt[i].gid+","+
					+ gis_sopt[i + 1].gid+","
					+ "(select geom from objeto where gid = "+gis_sopt[i].gid+"),"
					+ "(select geom from objeto where gid = "+gis_sopt[i + 1].gid+")"
					+ ")";
			
			DatabaseUtil.execute(sql);
			
			manageIndex(gis_sopt[i].gid);
			
			indexManager++;
		}
		
		manageIndex(gis_sopt[n].gid);
		
		sql = "INSERT INTO EUCLIDEAN_SEQUENCE(INDICE,OBJETO_GID,OBJETO_NEXT,OBJETO_GEOM,OBJETO_NEXT_GEOM) VALUES("
				+ (indexManager)+","+
				+ gis_sopt[n].gid+","+
				+ gis_sopt[1].gid+","
				+ "(select geom from objeto where gid = "+gis_sopt[n].gid+"),"
				+ "(select geom from objeto where gid = "+gis_sopt[1].gid+")"
				+ ")";
		
		DatabaseUtil.execute(sql);
		
		if(makeDisjkstranWay)
			gis_cost = buildDijkstranWay(gis_sopt);
		
		return gis_cost;
		
	}
	
	public static Double buildDijkstranWay(Place[] gis_sopt) throws Exception {
		
		Double gis_cost = 0.0;

		for (int i = 1; i < n; i++) {
		
				gis_cost += buildDijkstranStretch(gis_sopt[i], gis_sopt[i + 1],false);
				searchForBridge = true;
			
		}
		
		gis_cost += buildDijkstranStretch(gis_sopt[n], gis_sopt[1],false);
		searchForBridge = true;

		return gis_cost;
		
	}


	public static Double buildDijkstranStretch(Place place1, Place place2,boolean isBridge)
			throws Exception {
		
		ArrayList<Node> path = null;
		
		Dijkstra.cost = Double.POSITIVE_INFINITY;
		
		int surplus = 1;
		
		//aumentar o box caso n�o retorne um caminho
		if(place1.x != place2.x && place1.y != place2.y){
		while(Dijkstra.cost == Double.POSITIVE_INFINITY && surplus <= 10){

			buildGraphFromBox(new GISBox(place1, place2,getDistance(place1.gid, place2.gid),surplus));
			
			graphManager.put("start", start == null?graphManager.get(findStartNode(place1)):graphManager.get(start.name));
			
			List<String> lastNodes = findLastNodes(place2,isBridge);  
		
			graphManager.put("penult", graphManager.get(lastNodes.get(0)));
			
			graphManager.put("last", graphManager.get(lastNodes.get(1)) == null?graphManager.get(lastNodes.get(0)):graphManager.get(lastNodes.get(1)) );
			
			if(graphManager.get("start")!=null && graphManager.get("penult") != null && graphManager.get("last") != null)
				path = Dijkstra.buildPath(graphManager);
			
			surplus++;
			
		}
		
		start  = graphManager.get("last");
		
		if(path !=null)
			recordPath(path);
		}
		else if(searchForBridge){
			searchForBridge = false;
			buildDijkstranStretch(place1, place2,true);
		}
		
		if(Dijkstra.cost == Double.POSITIVE_INFINITY)
			Dijkstra.cost=0.0;
		
		return Dijkstra.cost;

	}

	public static void buildGraphFromBox(GISBox box2d) throws Exception {

		String sql = "SELECT GID FROM new_node WHERE (ST_X(GEOM) BETWEEN "
				+ box2d.left
				+ " AND "
				+ box2d.right
				+ " ) and (ST_Y(GEOM) BETWEEN "
				+ box2d.bottom
				+ " AND "
				+ box2d.top + ")";

		ResultSet rs = DatabaseUtil.executeQuery(sql);

		//Node[] nodes = new Node[rs.getFetchSize()];

		graphManager = new HashMap<String, Node>();
		
		//int count = 0;

		while (rs.next()) {
			//nodes[count] = new Node("" + rs.getInt(1),rs.getInt(1));
			graphManager.put("" + rs.getInt(1), new Node("" + rs.getInt(1),rs.getInt(1)));
		}
		
		HashMap<String, Node> graphM = graphManager;
		
		setAdjacencies();
		//nodeManager.values().toArray()

		//return nodes;
	}

	public static void setAdjacencies()
			throws Exception {
		 Set<String> keys = graphManager.keySet();
		 
		for(String key : keys){
		
			String sql = "SELECT NEXT FROM new_node WHERE GID = "+key;

			ResultSet rs = DatabaseUtil.executeQuery(sql);
			
			while(rs.next()){
				graphManager.get(key).adjacencies = setAdjacency(key, rs.getString(1));
			}
			
			//graphManager.put(""+node.gi, node);
			
		}
		
	}
	
	public static Edge[] setAdjacency(String nodeId, String adjacencies)
			throws Exception {

		String[] nexters = adjacencies.split(";");

		Edge[] edges = new Edge[nexters.length];

		int count = 0;
		
		for (String next : nexters) {

			String sql = "SELECT (ST_LENGTh(GEOM) * 0.001),GID FROM new_edge WHERE (NODES LIKE '"+next+"_"+nodeId+"' OR NODES LIKE '"+nodeId+"_"+next+"')";

			ResultSet rs = DatabaseUtil.executeQuery(sql);

		//	Node[] nodes = new Node[rs.getFetchSize()];

			while (rs.next()) {

				edges[count] = new Edge(rs.getInt(2),graphManager.get(next),rs.getDouble(1));
				
			}
			
			count++;
			
		}
		return edges;
	}

	public static String findStartNode(Place place) throws Exception {
		
		//if(place.gid == 1)
		//	return "12774";
		

		String sql = "select GID from new_node order by St_distance(geom,(select geom from new_edge order by St_distance(geom,(select geom from objeto where gid = "+place.gid+")) limit 1)) limit 1";

		ResultSet rs = DatabaseUtil.executeQuery(sql);

		int gid = 0;

		while (rs.next()) {
			gid = rs.getInt(1);
		}

		return ""+gid;
	}

	public static List<String> findLastNodes(Place place,boolean isBridge) throws Exception {

		List<String> nodes = new ArrayList<String>();
		
		//if(place.gid == 1){
			
		//	nodes.add("12855");
		//	nodes.add("12774");
			
		//	return nodes;
		//}

		ResultSet rs = DatabaseUtil.executeQuery("select gid,one_way from new_edge order by St_distance(geom,(select geom from objeto where gid = "+place.gid+")) limit 1");

		List<Object> data = new ArrayList<Object>(); 
		
		while (rs.next()) {
			data.add(rs.getInt(1));
			data.add(rs.getString(2));
		}

		String way = data.get(1).equals("FT")?"DESC":"ASC";
		
		if(isBridge)
			rs = DatabaseUtil.executeQuery("select gid from new_node where VIA1_NAME IS NULL and VIA2_NAME IS NULL AND St_Intersects(geom,( select geom from new_edge where gid = "+data.get(0)+")) order by geom "+way);
		else
			rs = DatabaseUtil.executeQuery("select gid from new_node where VIA1_NAME IS NOT NULL and VIA2_NAME IS NOT NULL AND St_Intersects(geom,( select geom from new_edge where gid = "+data.get(0)+")) order by geom "+way);

		while (rs.next()) {
			nodes.add(rs.getInt(1)+"");
		}
		
		//para arestas que tem apenas um nó
		if(nodes.size()<2)
			nodes.add(nodes.get(0));

		return nodes;
	}

	public static void recordPath(ArrayList<Node> nodes)
			throws Exception {


		for(int i =0;i<nodes.size()-1;i++){
			
			GISManager.dijkstraIndex ++;
			
			String sql = "INSERT INTO DIJKSTRAN_SEQUENCE(INDICE,EDGE_GID) VALUES("+GISManager.dijkstraIndex+",(SELECT GID FROM new_edge WHERE (NODES LIKE '"+nodes.get(i).gid+"_"+nodes.get(i+1).gid+"' OR NODES LIKE '"+nodes.get(i+1).gid+"_"+nodes.get(i).gid+"') LIMIT 1))";
			
			DatabaseUtil.execute(sql);
			
		}
		
			
	}

	public static void resetPath()
			throws Exception {


		String sql = "Delete from DIJKSTRAN_SEQUENCE";
		DatabaseUtil.execute(sql);
		DatabaseUtil.execute("DELETE FROM EUCLIDEAN_SEQUENCE");
		
			
	}
	
	public static void recordProcess(String h, int size, double time, double solution,double gis_cost)
			throws Exception {


		String sql = "INSERT INTO historico(heuristica,n,tempo,custo,custo_gis) values('"+h+"',"+size+","+time+","+(solution/10)+","+(gis_cost)+")";
		DatabaseUtil.execute(sql);
		
		//heuristica text,
		  //n integer,
		  //tempo double precision,
		  //solucao double precision,
			
	}
	
	public static void retrieveHistory(String h,PrintWriter pw)
			throws Exception {


		String sql = "SELECT n, tempo, custo,custo_gis,gid FROM HISTORICO WHERE heuristica LIKE '%HIMB%' ORDER BY gid DESC limit 1 ";
		ResultSet rs = DatabaseUtil.executeQuery(sql);
	
		pw.print("<table cellspacing='0' id='the-table' style='width: 244px;'><thead><tr style='background:#eeeeee;'><th style='width: 60px;'>n</th><th style='width: 60px;'>Tempo de Construção(s)</th><th style='width: 60px;'>Custo GIS(km's)</th></tr></thead><tbody>");
		
		while (rs.next()) {
			pw.print("<tr>");
			pw.print("<td style='width: 60px;'>"+	rs.getInt(1)	+"</td>");
			pw.print("<td style='width: 60px;'>"+	rs.getDouble(2)	+"</td>");
			//pw.print("<td style='width: 60px;'>"+	rs.getDouble(3)	+"</td>");
			pw.print("<td style='width: 60px;'>"+	rs.getDouble(4)	+"</td>");
			pw.print("</tr>");
		}
		
		pw.print("</tbody></table>");
		
		//pw.print("<script type='text/javascript'>var tableGrid = new Ext.ux.grid.TableGrid('the-table', {stripeRows: true});tableGrid.render();window.alert('Hi there.');</script>");
		
	}
	
	public static void doIt()
			throws Exception {

		String sql = "select gid from bielsa LIMIT 199";

		ResultSet rs = DatabaseUtil.executeQuery(sql);

		int i = 1;
		
		sql = "DELETE FROM OBJETO WHERE GID > 1";
		DatabaseUtil.execute(sql);

		while (rs.next()) {
			sql = "INSERT INTO OBJETO(gid,NOME,ATIVO,GEOM) VALUES ("+(i+1)+",'cliente "+i+"',true,(SELECT GEOM FROM FORTALEZA_NODE WHERE GID = "+rs.getInt(1)+"))";
			DatabaseUtil.execute(sql);
			i++;
		}

		
	}
	
	public static int getDistance(int id1,int id2)
			throws Exception {

		String sql = "select st_distance(ob1.geom,ob2.geom)::integer / 5 from objeto as ob1,objeto as ob2 where ob1.gid = "+id1+" and ob2.gid="+id2;

		ResultSet rs = DatabaseUtil.executeQuery(sql);
		
		int d = 0;

		while (rs.next()) {
			d = rs.getInt(1);
		}


		if(d > 200)
			return 300;
		else if(d ==0 || d > 100)
			return 100;
		else
			return d;
			
		//return 100;
		
	}
	
	public static void manageIndex(int gid)
			throws Exception {

		String sql = "select gid from cliente_ativo where geom = (select geom from objeto where gid = "+gid+") and gid <> "+gid;

		ResultSet rs = DatabaseUtil.executeQuery(sql);

		while (rs.next()) {
		//	d = rs.getInt(1);
			
			indexManager++;
			
			sql = "INSERT INTO EUCLIDEAN_SEQUENCE(INDICE,OBJETO_GID) VALUES("+indexManager+","+ rs.getInt(1)+ ")";
			
			DatabaseUtil.execute(sql);
			
		}
			
		//return 100;
		
	}
	
	public static void main(String args[]) throws Exception{
		//doIt();
	}

	
}
