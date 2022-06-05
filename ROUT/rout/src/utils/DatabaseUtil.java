package utils;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.LinkedList;

public class DatabaseUtil {
	
	static public Connection conn = null;
	
	
	public static void openConnection() throws Exception
	  {
	    
		//Connection conn = null;
	    
		Class.forName("org.postgresql.Driver");
	    
		String url = "jdbc:postgresql://localhost:5432/coelce";
	    
		conn = DriverManager.getConnection(url,"postgres", "postgres");
	    
	    //return conn;
	    
	  }
	
	public static ResultSet executeQuery(String sql) throws Exception
	  {
		  if(conn == null)
			  openConnection();
		  
		//  ResultSet rs = conn.createStatement().executeQuery("SELECT id, subject, permalink FROM blogs ORDER BY id");
		
	      return conn.createStatement().executeQuery(sql);
	      
	      
	      
//	      while ( rs.next() )
	//      {
	      //  Object blog = new Object();
	      //  blog.id        = rs.getInt    ("id");
	       // blog.subject   = rs.getString ("subject");
	       // blog.permalink = rs.getString ("permalink");
//	        listOfBlogs.add(blog);
	  //    }
	    //  rs.close();
	    
	  }
	
	public static boolean execute(String sql) throws Exception
	  {
		  if(conn == null)
			  openConnection();
		  
		//  ResultSet rs = conn.createStatement().executeQuery("SELECT id, subject, permalink FROM blogs ORDER BY id");
		
	      return conn.createStatement().execute(sql);
	      
	      
	      
//	      while ( rs.next() )
	//      {
	      //  Object blog = new Object();
	      //  blog.id        = rs.getInt    ("id");
	       // blog.subject   = rs.getString ("subject");
	       // blog.permalink = rs.getString ("permalink");
//	        listOfBlogs.add(blog);
	  //    }
	    //  rs.close();
	    
	  }
	


}
