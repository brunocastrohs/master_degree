package heurisken;

import org.apache.commons.collections.primitives.ArrayIntList;
import org.apache.commons.collections.primitives.IntList;

public class OPT4 extends Heurisken {

	//private static int[] s;
	// teaser
	public static IntList listL;

	public static void optimize(int ss[]) {
		int i, j, k,  p;
		int s[] = new int[MAXn];
		 //  clock_t ticks;

		   for (i=1; i<=n; i++) s[i]=ss[i];
		   for (i=1; i<=(n-2); i++) {
		      for (j=i+1; j<=(n-1); j++) {
		         for (k=j+1; k<=n; k++)  {
		           // 1a permuta��o (ikj), avalia e se desfaz dela
		           p=s[j]; s[j]=s[k]; s[k]=p;
		           g(s);
		           s[k]=s[j]; s[j]=p;

		           // 2a permuta��o (jik), avalia e se desfaz dela
		           p=s[i]; s[i]=s[j]; s[j]=p;
		           g(s);
		           s[j]=s[i]; s[i]=p;

		           // 3a permuta��o (jki), avalia e se desfaz dela
		           p=s[i]; s[i]=s[j]; s[j]=s[k]; s[k]=p;
		           g(s);
		           s[k]=s[j]; s[j]=s[i]; s[i]=p;

		           // 4a permuta��o (kij), avalia e se desfaz dela
		           p=s[i]; s[i]=s[k]; s[k]=s[j]; s[j]=p;
		           g(s);
		           s[j]=s[k]; s[k]=s[i]; s[i]=p;

		           // 5a permuta��o (kji), avalia e se desfaz dela
		           p=s[i]; s[i]=s[k]; s[k]=p;
		           g(s);
		           s[k]=s[i]; s[i]=p;
		           
		           
		         } // end for k
		      } // end for j
		   } // end for i
		
	}

	static void g(int ss[])
	{
	    int i, custo=0;

	    for (i = 1; i < n; i++) {
	       custo= custo + D[ ss[i] ][ ss[i+1] ];
	    }
	    custo= custo + D[ ss[n] ][ ss[1] ]; 

	    if (custo<custo_opt) {
	       custo_opt=custo;
	       for (i = 1; i <= n; i++) sopt[i]=ss[i];
	    }
	}
	

}
