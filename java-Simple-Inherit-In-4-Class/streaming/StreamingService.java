import java.util.ArrayList;


public class StreamingService 
{	
	private ArrayList<DigitalContent> list;

	public StreamingService() {
		this.list = new ArrayList<>();
	}

	public void add(DigitalContent d) {
		this.list.add(d);
	}

	/**
	 * 
	 * returns a list of all the matching digital content available on the streaming service
	 * that matches the query string.
	 * 
	 * */

	public ArrayList<DigitalContent> match(String query)
	{
		ArrayList<DigitalContent> ret = new ArrayList<>();
		for (DigitalContent d : this.list) {
			if ( d.match(query) ) {
				ret.add(d);
			}
		}
		return ret;
	}
	
	
}
