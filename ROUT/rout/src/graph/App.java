package graph;


import java.util.HashMap;
import java.util.Map;

import dijkstra.Dijkstra;
import dijkstra.Node;
import utils.MatrixReader;
import utils.Place;
import utils.TimeManager;
import gis.GISManager;
import heurisken.HIB;
import heurisken.Heurisken;
import heurisken.OPT4;

public class App {

	public static void runGIS(int kume) throws Exception {

		Heurisken.init(kume);

		TimeManager t = new TimeManager("RUN");

		boolean makeDisjkstranWay = true;

		HIB.optimize();
		System.out.print("(" + t.getElapsedTime() + "s)");
		System.out.println("HIB: " + HIB.custo_opt);
		Heurisken.printSopt();
		//OPT4.optimize(HIB.sopt);
		//System.out.print("(" + t.getElapsedTime() + "s)");
		//System.out.println("HIMB-OPT4: " + OPT4.custo_opt);
		//OPT4.printSopt();
		Heurisken.buildGISSopt(makeDisjkstranWay);
		GISManager.recordProcess("HIMB", Heurisken.n, t.getElapsedTime(),Heurisken.custo_opt, Heurisken.gis_custo_opt);
				
		resetApp();

	}

	public static void runTest(int kume) throws Exception {

		Heurisken.init(kume);

		TimeManager t = new TimeManager("RUN");

		boolean makeDisjkstranWay = true;

		HIB.optimize();
		System.out.print("(" + t.getElapsedTime() + "s)");
		System.out.println("HIB: " + HIB.custo_opt);
		Heurisken.printSopt();
		//OPT4.optimize(HIB.sopt);
		//System.out.print("(" + t.getElapsedTime() + "s)");
		//System.out.println("HIMB-OPT4: " + OPT4.custo_opt);
		//OPT4.printSopt();
		Heurisken.buildGISSopt(makeDisjkstranWay);
		GISManager.recordProcess("HIMB", Heurisken.n, t.getElapsedTime(),Heurisken.custo_opt, Heurisken.gis_custo_opt);
				
		resetApp();

	}

	static public int[] soptToOPT4(int[] ss) {

		int i, j;
		int[] s = new int[Heurisken.MAXn];

		j = 1;
		for (i = 1; i <= Heurisken.n; i++) {
			s[i] = ss[j];
			j = ss[j];
		}
		return s;

	}

	public static void main(String[] args) throws Exception {
		runTest(5);
	}
	
	public static void resetApp() throws Exception {
		
		//Heurisken
		Heurisken.MAXn = 551;
		Heurisken.D = new int[Heurisken.MAXn][Heurisken.MAXn];
		Heurisken.n = 0; Heurisken.cont = 0;
		Heurisken.custo_opt = 34000000;
		Heurisken.sopt = new int[Heurisken.MAXn];
		Heurisken.set = new int[Heurisken.MAXn];
		Heurisken.places = null;
		Heurisken.gis_sopt = null;
		Heurisken.gis_custo_opt = null;
		
		//MATRIX
		MatrixReader.n = 0;
		MatrixReader.D = null;
		MatrixReader.places = null;
		MatrixReader.gis_sopt = null;
		
		//DIJSKSTRA
		Dijkstra.cost = Double.POSITIVE_INFINITY;
		
		//GSIManager
		GISManager.n = 0;

		GISManager.places = null;
		
		GISManager.indexManager = 1;

		GISManager.D = null;
		
		GISManager.graphManager = null;
		
		GISManager.edgeManager = null;
		
		GISManager.start = null;
		
		GISManager.dijkstraIndex = 0;
		
		//OPT4
		OPT4.listL = null;

		
	}

}
