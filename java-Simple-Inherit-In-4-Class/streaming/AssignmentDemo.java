/**
 * 
 * 
 * Assignment Demo
 * 
 * 
 * */
import java.util.ArrayList;
import java.util.Collections;

public class AssignmentDemo 
{

	 public static void outputQueryResult(StreamingService streaming,String query)
	 {
	 System.out.println("----------------------------------------");
	 ArrayList<DigitalContent> matches = streaming.match(query);

	 System.out.println("Matching query: \""+query+"\"");
	 streaming.match("Orson"); Collections.sort(matches); for(DigitalContent dc :
	 matches) { System.out.println(dc); }
	 System.out.println("Total of: "+matches.size()+" matches");
	 System.out.println("----------------------------------------"); }

	public static void main(String[] args) 
	{
		

		
		System.out.println("Welcome to the Streaming Service");

		//instantiate film and music content and add to digital service providers
		Film bigSleep = new Film("The Big Sleep","Warner Bros.","1946","Humphrey Bogart, Lauren Bacall");
		Film keyLargo = new Film("Key Largo","Warner Bros.","1948","Humphrey Bogart, Lauren Bacall");
		Film thirdMan = new Film("The Third Man","London Film Productions","1949","Orson Welles, Joseph Cotten");
		Film citizenKane = new Film("Citizen Kane","RKO Radio Pictures","1941","Orson Welles, Joseph Cotten");
		Film doubleIndemnity = new Film("Double Indemnity","Paramount Pictures","1944","Fred MacMurray, Barbara Stanwyck");
		Film sunsetBlvd = new Film("Sunset Blvd.","Paramount Pictures","1950","William Holden, Gloria Swanson");

		Music bruce = new Music("Born in the USA", "Bruce Springsteen", "1984", "Bruce Springsteen");
		Music vitalogy = new Music("Vitalogy", "Sony", "1994", "Pearl Jam");
		Music ten = new Music("Ten", "Sony", "1991", "Pearl Jam");
		Music elton = new Music("Goodbye Yellow Brick Road","This","1973","Elton John");
		Music weezer = new Music("Weezer","DGC Records","1994","Weezer");

		StreamingService streaming = new StreamingService();
		//add content to the streaming service
		streaming.add(bigSleep);
		streaming.add(keyLargo);
		streaming.add(thirdMan);
		streaming.add(citizenKane);
		streaming.add(doubleIndemnity);
		streaming.add(sunsetBlvd);

		streaming.add(bruce);
		streaming.add(vitalogy);
		streaming.add(elton);
		streaming.add(weezer);
		streaming.add(ten);

		outputQueryResult(streaming,"orson");
		outputQueryResult(streaming,"pearl jam");
		outputQueryResult(streaming,"Bruce");
		outputQueryResult(streaming,"194");

	}

}

/**
Welcome to the Streaming Service
----------------------------------------
Matching query: "orson"
Title: Citizen Kane Publisher: RKO Radio Pictures Released: 1941 Cast: Orson Welles, Joseph Cotten
Title: The Third Man Publisher: London Film Productions Released: 1949 Cast: Orson Welles, Joseph Cotten
Total of: 2 matches
----------------------------------------
----------------------------------------
Matching query: "pearl jam"
Title: Ten Publisher: Sony Released: 1991 Artist: Pearl Jam
Title: Vitalogy Publisher: Sony Released: 1994 Artist: Pearl Jam
Total of: 2 matches
----------------------------------------
----------------------------------------
Matching query: "Bruce"
Title: Born in the USA Publisher: Bruce Springsteen Released: 1984 Artist: Bruce Springsteen
Total of: 1 matches
----------------------------------------
----------------------------------------
Matching query: "194"
Title: Citizen Kane Publisher: RKO Radio Pictures Released: 1941 Cast: Orson Welles, Joseph Cotten
Title: Double Indemnity Publisher: Paramount Pictures Released: 1944 Cast: Fred MacMurray, Barbara Stanwyck
Title: Key Largo Publisher: Warner Bros. Released: 1948 Cast: Humphrey Bogart, Lauren Bacall
Title: The Big Sleep Publisher: Warner Bros. Released: 1946 Cast: Humphrey Bogart, Lauren Bacall
Title: The Third Man Publisher: London Film Productions Released: 1949 Cast: Orson Welles, Joseph Cotten
Total of: 5 matches
----------------------------------------

 * 
 * 
 * **/



