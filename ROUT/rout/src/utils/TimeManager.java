package utils;


public class TimeManager {

	private long start;
	private String desc = "";
	
	public TimeManager() {
		start = System.currentTimeMillis();
	}
	
	public TimeManager(String desc) {
		start = System.currentTimeMillis();
		this.desc = desc;
	}
	
	public double getElapsedTime() {
		long end = System.currentTimeMillis() - start;
		
		return ((double)end / 1000);
		
	}
}
