package heurisken;

import gis.GISManager;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.NoSuchElementException;
import java.util.StringTokenizer;

import utils.DatabaseUtil;
import utils.Instance;
import utils.MatrixReader;
import utils.Place;

public class Heurisken {
	
	static public int MAXn = 551;
	static public int D[][] = new int[MAXn][MAXn];
	static public int n = 0, cont = 0;
	static public int custo_opt = 34000000;
	static public int sopt[] = new int[MAXn];
	static public int set[] = new int[MAXn];
	//gis
	static public Place places[];
	static public Place gis_sopt[];
	static public Double gis_custo_opt;
	
	public static void init(int kume) throws Exception {

		MatrixReader.readFromGIS(kume);
		D = MatrixReader.D;
		n =	MatrixReader.n;
		places = MatrixReader.places;
		

	}
			
	static void g(int ss[]) {
		int i, custo = 0;

		for (i = 1; i < n; i++) {
			custo = custo + D[ss[i]][ss[i + 1]];
		}
		custo = custo + D[ss[n]][ss[1]];

		if (custo < custo_opt) {
			custo_opt = custo;
			System.out.println("S: "+custo_opt);
			for (i = 1; i <= n; i++)
				sopt[i] = ss[i];
		}
	}

	static int vizinho(int k, int p)
	{
	   int i, j=0, min=20000;

	   for(i=1; i<=n; i++) {
	     if ((k!=i) && (p!=i)) {
	        if ((D[k][i]<min) && (set[i]!=1)) {
	          min= D[k][i];
	          j=i;
	        }
	     }
	   } // end i
	   return j;
	}
	
	static int vizinho(int p)    // Determina o Vizinho mais pr�ximo do n� p
	{
	   int i, j, min;

	   j=0;
	   min=20000;
	   for (i=1; i<=n; i++) {
	     if (set[i]==0) {
	        if (D[p][i]<min) {
	          min=D[p][i];
	          j=i;
	        }
	     }
	   } // end for i
	   return j;
	}
	
	static void gHVPs(int ss[]) {
		int i, custo=0;

	    for (i = 1; i <= n; i++) {
	       custo= custo + D[i][ ss[i] ];
	    }

	    if (custo<custo_opt) {
	       custo_opt=custo;
	       System.out.println("S: "+custo_opt);
	       for (i = 1; i <= n; i++)
	    	   sopt[i]=ss[i];
	    }
		
	}
	
	static void OptHVPs(int ss[])
	{
	    int i, j;
	    int[] s = new int[MAXn];

	    j=1;
	    for (i = 1; i <= n; i++) {
	       s[i]=ss[j];
	       j=ss[j];
	    }
	    OPT4.optimize(s);
	}

	static void rotate(int a1[], int a2[])
	{
		int  j, ant, suc;
		int rota[] = new int[MAXn];

	   // Assign the rotate
	   j=1;
	   ant=a2[1];
	   suc=a1[1];
	   rota[ant]= suc;
	   while (j<=n) {
	     if (((a1[j] == suc) && (a2[j]!= ant)) || ((a2[j] == suc) && (a1[j]!= ant))) {
	         ant=suc;
	         if (a1[j] == suc) suc=a2[j]; else suc=a1[j];
	         rota[ant]= suc;
	         if (suc==a2[1]) j=n+1; else j=0;
	     }
	     j=j+1;
	   }

	   //printf("\nA rota e: 1-");
	   ant=1; suc=1;
	   for (j = 1; j <= n; j++) {
	      //if (j!=n) printf("%d-", rota[ant]); else printf("%d", rota[ant]);
	      sopt[suc]=rota[ant];
	      suc=suc+1;
	      ant=rota[ant];
	   }

	}
	
	static public void printSopt(){
		
		int sum = 0;
		
		for (int i = 1; i <= n; i++) {
			if(i >1)
				sum += D[sopt[i-1]][sopt[i]];
		    
			System.out.print(sopt[i]+"->");
		        
		}
		sum += D[sopt[n]][sopt[1]];
		System.out.println(sopt[1]+"="+sum);
	       
	}
	
	static public void printSopt(int[] so){
		
		int sum = 0;
		
		for (int i = 1; i <= n; i++) {
			if(i >1)
				sum += D[so[i-1]][so[i]];
		    
			System.out.print(so[i]+"->");
		        
		}
		sum += D[so[n]][so[1]];
		System.out.println(so[1]+"="+sum);
	       
	}
		
	static public void buildGISSopt(boolean makeDisjkstranWay) throws Exception{
		
		gis_sopt = new Place[n+2]; 
		
		for (int i = 1; i <= n; i++) {
			gis_sopt[i] = places[sopt[i]];    
		//	System.out.print(gis_sopt[i].gid+"->");
		        
		}
		gis_sopt[n+1] = places[sopt[1]];
		//sum += D[sopt[n]][sopt[1]];
	//	System.out.println(gis_sopt[n+1].gid);
		gis_custo_opt = GISManager.buildEuclideanWay(gis_sopt,makeDisjkstranWay) ;  
	}

	
}
