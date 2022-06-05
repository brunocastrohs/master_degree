package heurisken;

import java.util.ArrayList;
import java.util.List;

public class HIB extends Heurisken {

	static int[] s1 = new int[MAXn];
	static int addCount = 0;
	static int currentOptCost = 0;
	static ArrayList<Integer> P =  new ArrayList<Integer>();
	
	public static void optimize() {

		int i;
		
		
		for (i = 1; i <= n; i++) {
			P =  new ArrayList<Integer>();
			initP();
			//v0
			P.add(i);
			set[i] = 1;
			//vk
			int vk = vizinho(i, 0);
			P.add(vk);
			set[vk] = 1;
			addCount =2;
			// Faz o mesmo para as cabe�as c1 e c2 at� cont=n
			for (int k = 1; k <= n; k++) {
				//Inserir k em P
				if(set[k]!=1){
					vizinhoHIB(k);
					set[k] = 1;
					addCount ++;
					
				}
				
			} 

			for(int l = 1; l <= n; l++) {
				s1[l] = P.get(l);
			}
			
			g(s1);
	
		}
		
	} 
	
	public static void vizinhoHIB(int k) {
	
		int i,x,y,kx = 1, min=20000;

		for(i=1; i<=addCount-1; i++) {
			   
			x = i;
			y = i+1;
			
		    if ((k!=P.get(x)) && (k!=P.get(y)) ) {
		  
		    	int aux = D[P.get(x)][k]+D[k][P.get(y)]-D[P.get(x)][P.get(y)]; 
		    	
		    	if (aux<min) {
		      
		    		min = aux;
		    		kx = y;
		    		
		    	}
		    }
		  } 
		
		P.add(kx, k);
		
	}
	
	public static void initP() {
		
		addCount = 0;
		
		P.add(0);
		
		for (int i = 1; i <= n+1; i++) {
			//Inserir k em P
		//	P.add(0);
			set[i] = 0;
		} 
		
	}
	
		
}