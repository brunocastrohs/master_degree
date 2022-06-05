package gis;

import utils.Place;

public class GISBox {
	
	public Double left = 0.0,right = 0.0, top =0.0,bottom =0.0;
	
	public GISBox(Place place1,Place place2,int d, int surplus) throws Exception{
	
		Integer c = d;
		
		if(place1.x < place2.x){
			left = place1.x-(c*surplus);
			right = place2.x+(c*surplus);
		}else{
			left = place2.x-(c*surplus);
			right = place1.x+(c*surplus);
		}
		
		
		if(place1.y > place2.y){
			top = place1.y+(c*surplus);
			bottom = place2.y-(c*surplus);
		}else{
			top = place2.y+(c*surplus);
			bottom = place1.y-(c*surplus);
		}
		
		
	}
	
	

}
