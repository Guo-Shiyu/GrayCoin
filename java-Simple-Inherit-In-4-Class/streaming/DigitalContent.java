import java.util.Locale;

/**
 * 
 * 
 * 
 * 
 * */


abstract public class DigitalContent implements Comparable
{
	private String title;
	private String publisher;
	private String releaseDate;

	public DigitalContent(String title, String publisher, String releaseDate) {
		this.title = title;
		this.publisher = publisher;
		this.releaseDate = releaseDate;
	}

	public String getTitle() {
		return title;
	}

	public String getReleaseDate() {
		return releaseDate;
	}

	public void setReleaseDate(String releaseDate) {
		this.releaseDate = releaseDate;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public String getPublisher() {
		return publisher;
	}

	public void setPublisher(String publisher) {
		this.publisher = publisher;
	}

	@Override
	public String toString() {
		return  "Title: " + title +
				", Publisher: " + publisher +
				", Released: " + releaseDate;
	}

	/**
	 * 
	 * Returns true if the content's title, publisher or release date contains the query string
	 * It is not case sensitive.
	 * 
	 * */
	public boolean match(String query)
	{
		String target = query.toLowerCase(Locale.ROOT);
		String self = this.toString().toLowerCase(Locale.ROOT);
		return self.contains(target);
	}

	@Override
	public int compareTo(Object o) {
		return this.title.compareTo(((DigitalContent) o).title);
	}
}
