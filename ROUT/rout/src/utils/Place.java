package utils;


/**
 * 
 *  $Id$ 
 *
 */
public class Place {
	public final int gid;
	public final Double x;
	public final Double y;
	public boolean isVisited;

	public Place(final int gid, final Double x, final Double y) {
		this.gid = gid;
		this.x = x;
		this.y = y;
	}
	//dist�ncia euclidiana bidimensional
	public static int distance(final Place city1, final Place city2) {
		
		double x = city2.x - city1.x;
		
		double y = city2.y - city1.y;
		
		double result = Math.sqrt((x * x) + (y * y));
		
		return (int) (result+0.2);
		
	}

	public Place(final Place city) {
		this.gid = city.gid;
		this.x = city.x;
		this.y = city.y;
	}

	@Override
	public boolean equals(final Object o) {
		if (o instanceof Place) {
			final Place other = (Place) o;
			return gid == other.gid && x == other.x && y == other.y;
		}
		return false;
	}

	@Override
	public String toString() {
		return gid + ";  [x="+x+"; y="+y+"]" ;
	}

}
