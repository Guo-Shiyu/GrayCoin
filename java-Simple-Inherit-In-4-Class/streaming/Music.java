import java.util.Locale;

public class Music extends DigitalContent
{
	private String artist;

	public Music(String title, String publisher, String releaseDate, String artist) {
		super(title, publisher, releaseDate);
		this.artist = artist;
	}

	public String getArtist() {
		return artist;
	}

	public void setArtist(String artist) {
		this.artist = artist;
	}

	@Override
	public String toString() {
		return 	super.toString() +
				" Artist: " + artist;
	}

	/**
	 * 
	 * Returns true if the content's title, publisher  release date or artist contains the query string
	 * It is not case sensitive.
	 * 
	 * */

	@Override
	public boolean match(String query)
	{
		String target = query.toLowerCase(Locale.ROOT);
		String self = this.toString().toLowerCase(Locale.ROOT);
		return self.contains(target) && super.match(target);
	}
}
