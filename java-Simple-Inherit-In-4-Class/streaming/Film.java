import java.util.Locale;

public class Film extends DigitalContent
{
	private String cast;

	public String getCast() {
		return cast;
	}

	public void setCast(String cast) {
		this.cast = cast;
	}

	@Override
	public String toString() {
		return super.toString() + " Cast: " + cast;
	}

	public Film(String title, String publisher, String releaseDate, String cast) {
		super(title, publisher, releaseDate);
		this.cast = cast;
	}

	/**
	 * 
	 * Returns true if the content's title, publisher  release date or cast contains the query string
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
