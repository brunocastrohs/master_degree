package utils;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;


/**
 * 
 *  $Id$
 * 
 */
public class Instance {

	private Place[] places;
	private int n;
	private int D[][];
	//teaser
	public int nodes[];
	
	public Instance(String path2file,boolean hasIndex) throws IOException {
		readPoints(path2file,hasIndex);
		setDistanceWays();
		//setDistanceWays();
		//readTeaser(path2file);
	}

	private Place[] readPoints(String path2file,boolean hasIndex) throws IOException {

		BufferedReader reader = new BufferedReader(new FileReader(path2file));
		
		int size = Integer.parseInt(reader.readLine().trim());
		
		n = size;
		places = new Place[n+1];
		//cities[](new City(0, 0, 0));
		for (int i = 1; i <= size; i++) {

			String r = reader.readLine();
			StringTokenizer token = new StringTokenizer(r, " \t");

			if(hasIndex)
				token.nextToken().trim();
			
			Double x = Double.parseDouble(token.nextToken().trim());
			Double y = Double.parseDouble(token.nextToken().trim());

			Place place = new Place(i, x, y);
			places[i] = place;
		}

		return places;
	}
	
	private void setDistanceWays() {

		D = new int[n+1][n+1];
		for (int i = 1; i < places.length; i++) {
			for (int j = i + 1; j < places.length; j++) {
				D[i][j] = Place.distance(places[i], places[j]);
				D[j][i] = D[i][j];
			}
		}
	}
	
	public int[][] getWays() {
		return D;
	}

	public int getInstSize() {
		return n;
	}

	

}
