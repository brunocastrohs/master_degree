package utils;

import gis.GISManager;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.StringTokenizer;

public class MatrixReader {

	public static int n = 0;
	public static int[][] D;
	
	//gis
	static public Place places[];
	static public Place gis_sopt[];
		
	public static void readFromGIS(int kume)throws Exception{
		D = GISManager.retrieveDGIS(kume);
		n = GISManager.n;
		places = GISManager.places;
		
	}
	
	public static void printD() throws Exception {

		for(int i = 1; i <= n; i++){
			System.out.print(" ("+i+") ");
			for(int j = 1; j <= n; j++){
				System.out.print(" "+D[i][j]+" ("+j+")");
			}
			System.out.println("");
		}
		
		
	}

	
	

}
