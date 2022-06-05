package servlet;

import gis.GISManager;
import graph.App;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.*;

public class ReceiverServlet extends HttpServlet {

	public void doPost(HttpServletRequest req, HttpServletResponse res)
			throws ServletException, IOException {
		
		res.addHeader("Access-Control-Allow-Origin", "*");
		res.addHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT, DELETE, HEAD");
	        
		
		PrintWriter out = res.getWriter();
		
		String requestType = req.getHeader("request");
		String idH = req.getHeader("idH");
		
		try{
		
			if(Integer.parseInt(requestType) == 1){
				App.runGIS(Integer.parseInt(idH));
				//out.print("YES");
			}
			else if(Integer.parseInt(requestType) == 2){
				GISManager.retrieveHistory(idH,out);
				//out.print("YES 2");
			}
			
		}
		catch(Exception e){
		 out.println(e.toString());
		 out.println(req.getHeader("request"));		 
		 out.println(req.getHeader("idH"));
		 out.println("Failure Happen!");
		}
		//out.println("Hello, world!");
		out.close();
	}

	
	protected void doGet(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		resp.addHeader("Access-Control-Allow-Origin", "*");
		resp.addHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PUT, DELETE, HEAD");
	   
		// TODO Auto-generated method stub
		PrintWriter out = resp.getWriter();
		
		String requestType = req.getParameter("request");
		String idH = req.getParameter("idH");
		
		//Object x = req.getParameter("idH");
		
		try{
		
			if(Integer.parseInt(requestType) == 1){
				App.runGIS(Integer.parseInt(idH));
				//out.print("YES");
			}
			else if(Integer.parseInt(requestType) == 2){
				GISManager.retrieveHistory(idH,out);
			///	out.print("YES");
			}
			
		}
		catch(Exception e){
		 out.println(e.toString());
		 out.println(req.getHeader("request"));		 
		 out.println(req.getHeader("idH"));
		 out.println("Failure Happen!");
		}
		//out.println("Hello, world!");
		out.close();
	}
	
}
