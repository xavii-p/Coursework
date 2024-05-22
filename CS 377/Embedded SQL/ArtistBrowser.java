/* THIS WORK WAS MY OWN WORK. IT WAS WRITTEN WITHOUT CONSULTING
WORK WRITTEN BY OTHER STUDENTS OR COPIED FROM ONLINE RESOURCES.
_Xavier_Pierce_ */

import java.sql.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Queue;
import java.util.Set;
import java.util.HashSet;
import java.util.LinkedList;

public class ArtistBrowser {

	/* A connection to the database */
	private Connection connection;

	/**
	 * The constructor loads the JDBC driver. No need to modify this.
	 */
	public ArtistBrowser() {
		try {
			Class.forName("org.postgresql.Driver");
		} catch (ClassNotFoundException e) {
			System.err.println("Failed to locate the JDBC driver.");
		}
	}

	/**
	* Establishes a connection to be used for this session, assigning it to
	* the private instance variable 'connection'.
	*
	* @param  url       the url to the database
	* @param  username  the username to connect to the database
	* @param  password  the password to connect to the database
	* @return           true if the connection is successful, false otherwise
	*/
	public boolean connectDB(String url, String username, String password) {
		try {
			this.connection = DriverManager.getConnection(url, username, password);
			return true;
		} catch (SQLException se) {
			System.err.println("SQL Exception: " + se.getMessage());
			return false;
		}
	}

	/**
	* Closes the database connection.
	*
	* @return true if the closing was successful, false otherwise.
	*/
	public boolean disconnectDB() {
		try {
			this.connection.close();
		return true;
		} catch (SQLException se) {
			System.err.println("SQL Exception: " + se.getMessage());
			return false;
		}
	}
	/**
	 * HELPER METHOD
	 * Returns the artist_id of a given artist
	 * 
	 *
	 * Returns null if the artist does not exist
	 *
	 * 
	 *    
	 * 
	 *
	 *
	 * @param artist  the name of the artist to find
	 * @return        the artist_id of the given artist
	 */
	public Integer artistExists(String artist) {
		try {
			PreparedStatement stmt = this.connection.prepareStatement(
				"SELECT artist_id " +
				"FROM Artist " +
				"WHERE name = ?"
				);
			stmt.setString(1, artist);
			ResultSet artistExists = stmt.executeQuery();
			while (artistExists.next()) {
				return artistExists.getInt("artist_id");
			}

		} catch (SQLException e) {
			System.err.println("SQL Exception: " + e.getMessage());
			return null;
		}
		return null;
	}

	/**
	 * Returns a sorted list of the names of all musicians who were part of a band
	 * at some point between a given start year and an end year (inclusive).
 	 *
	 * Returns an empty list if no musicians match, or if the given timeframe is invalid.
	 *
	 * NOTE:
	 *    Use Collections.sort() to sort the names in ascending
	 *    alphabetical order.
	 *		Use prepared statements.
	 *
	 * @param startYear
	 * @param endYear
	 * @return  a sorted list of artist names
	 */
	public ArrayList<String> findArtistsInBands(int startYear, int endYear) {

		if (startYear > endYear){ // invalid timeframe
			return new ArrayList<>();
		}

		ArrayList<String> sortedNames = new ArrayList<>();
		try {
			PreparedStatement stmt = this.connection.prepareStatement(
			"SELECT DISTINCT name " + 
			"FROM Artist a1 JOIN WasInBand w1 ON a1.artist_id = w1.artist_id " +
			"WHERE w1.start_year <= ? AND (end_year >= ? OR w1.end_year IS NULL)"
			);

			stmt.setInt(1, startYear);
			stmt.setInt(2, endYear);
			ResultSet result = stmt.executeQuery();
		
			while (result.next()){
				sortedNames.add(result.getString("name"));
			}

			// sort
			Collections.sort(sortedNames);
			
		} catch (SQLException e){ // catch error
			System.err.println("SQL Exception: " + e.getMessage());
			return new ArrayList<>();
		}

		return sortedNames;
		
	}


	/**
	 * Returns a sorted list of the names of all musicians and bands
	 * who released at least one album in a given genre.
	 *
	 * Returns an empty list if no such genre exists or no artist matches.
	 *
	 * NOTE:
	 *    Use Collections.sort() to sort the names in ascending
	 *    alphabetical order.
	 *		Use prepared statements.
	 *
	 * @param genre  the genre to find artists for
	 * @return       a sorted list of artist names
	 */
	public ArrayList<String> findArtistsInGenre(String genre) {

		ArrayList<String> sortedArtists = new ArrayList<>();
		Integer genreID = null;
		try {
			// make sure genre exists
			PreparedStatement stmt = this.connection.prepareStatement(
				"SELECT genre_id FROM Genre WHERE genre = ?"
				);
			stmt.setString(1, genre);
			ResultSet ID = stmt.executeQuery();
			while (ID.next()) {
				genreID = ID.getInt("genre_id");
			}
			
			if (genreID == null) {
				return new ArrayList<>();
			}

			// find albums with this genre_id and the associated artist_id/name
			PreparedStatement stmt2 = this.connection.prepareStatement(
				"SELECT DISTINCT name " +
				"FROM Album al JOIN Artist ar ON al.artist_id = ar.artist_id " +
				"WHERE genre_id = ?"
				);
			
			stmt2.setInt(1, genreID);
			ResultSet artists = stmt2.executeQuery();
			while (artists.next()) {
				sortedArtists.add(artists.getString("name"));
			}

			// sort
			Collections.sort(sortedArtists);
			
		} catch (SQLException e){
			System.err.println("SQL Exception: " + e.getMessage());
			return new ArrayList<>();
		}

		return sortedArtists;

	}


	/**
	 * Returns a sorted list of the names of all collaborators
	 * (either as a main artist or guest) for a given artist.
	 *
	 * Returns an empty list if no such artist exists or the artist
	 * has no collaborators.
	 *
	 * NOTE:
	 *    Use Collections.sort() to sort the names in ascending
	 *    alphabetical order.
	 *		Use prepared statements.
	 *
	 * @param artist  the name of the artist to find collaborators for
	 * @return        a sorted list of artist names
	 */
	public ArrayList<String> findCollaborators(String artist) {

		ArrayList<String> sortedCollaborators = new ArrayList<>();

		// see if artist exists
		Integer artistID = artistExists(artist);
		if (artistID == null){
			return new ArrayList<>();
		} else {
			
			try {
	
				// find collaborators for this artist as the main artist
				PreparedStatement stmt2 = this.connection.prepareStatement(
					"SELECT DISTINCT name " + // avoid duplicate names
					"FROM Collaboration c JOIN Artist a ON c.artist2 = a.artist_id " + // guest collabs
					"WHERE artist1 = ? " +
					"UNION " +
					"SELECT DISTINCT name " +
					"FROM Collaboration c2 JOIN Artist a2 ON c2.artist1 = a2.artist_id " + // main collabs
					"WHERE artist2 = ?"
					);
				stmt2.setInt(1, artistID);
				stmt2.setInt(2, artistID);
				ResultSet Collaborators = stmt2.executeQuery();
				while (Collaborators.next()) {
					sortedCollaborators.add(Collaborators.getString("name"));
				}
	
				// sort
				Collections.sort(sortedCollaborators);
				
			} catch (SQLException e){
				System.err.println("SQL Exception: " + e.getMessage());
				return new ArrayList<>();
			}
		}

		return sortedCollaborators;

	}


	/**
	 * Returns a sorted list of the names of all songwriters
	 * who wrote songs for a given artist (the given artist is excluded).
	 *
	 * Returns an empty list if no such artist exists or the artist
	 * has no other songwriters other than themself.
	 *
	 * NOTE:
	 *    Use Collections.sort() to sort the names in ascending
	 *    alphabetical order.
	 *
	 * @param artist  the name of the artist to find the songwriters for
	 * @return        a sorted list of songwriter names
	 */
	public ArrayList<String> findSongwriters(String artist) {

		ArrayList<String> songWriters = new ArrayList<>();

		// see if artist exists
		Integer artistID = artistExists(artist);
		if (artistID == null){
			return new ArrayList<>();
		} else {
			try {
				PreparedStatement stmt = this.connection.prepareStatement(
					"SELECT DISTINCT a2.name AS songwriter " +
                    "FROM Artist a1 " +
                    "JOIN Album al ON a1.artist_id = al.artist_id " +
                    "JOIN BelongsToAlbum bta ON al.album_id = bta.album_id " +
                    "JOIN Song s ON bta.song_id = s.song_id " +
                    "JOIN Artist a2 ON s.songwriter_id = a2.artist_id " +
                    "WHERE a1.artist_id = ? AND a2.artist_id != a1.artist_id"
					);
				
				stmt.setInt(1, artistID);
				ResultSet result = stmt.executeQuery();

				while (result.next()) {
					songWriters.add(result.getString("songwriter"));
				}

				// sort
				Collections.sort(songWriters);
				

			} catch (SQLException e) {
				System.err.println("SQL Exception: " + e.getMessage());
				return new ArrayList<>();
			}
		}
		return songWriters;
	}

	/**
	 * Returns a sorted list of the names of all common acquaintances
	 * for a given pair of artists.
	 *
	 * Returns an empty list if either of the artists does not exist,
	 * or they have no acquaintances.
	 *
	 * NOTE:
	 *    Use Collections.sort() to sort the names in ascending
	 *    alphabetical order.
	 *
	 * @param artist1  the name of the first artist to find acquaintances for
	 * @param artist2  the name of the second artist to find acquaintances for
	 * @return         a sorted list of artist names
	 */
	public ArrayList<String> findCommonAcquaintances(String artist1, String artist2) {


		// see if artists exist
		Integer A1ID = artistExists(artist1);
		Integer A2ID = artistExists(artist2);
		if (A1ID == null || A2ID == null){
			return new ArrayList<>();
		}

		if (A1ID.equals(A2ID)){
			return new ArrayList<>();
		}

		// find collaborators for both artists
		ArrayList<String> artist1Collabs = findCollaborators(artist1);
		ArrayList<String> artist2Collabs = findCollaborators(artist2);

		
		// find writers for both artists
		ArrayList<String> artist1Writers = findSongwriters(artist1);
		ArrayList<String> artist2Writers = findSongwriters(artist2);

		// combine collabs and writers
		artist1Collabs.addAll(artist1Writers);
		artist2Collabs.addAll(artist2Writers);

		// remove duplicate names
		Set<String> A1 = new HashSet<>(artist1Collabs);
		Set<String> A2 = new HashSet<>(artist2Collabs);

		// find common names
		A1.retainAll(A2);

		ArrayList<String> commonAcquaintances = new ArrayList<String>(A1);

		// sort
		Collections.sort(commonAcquaintances);
		return commonAcquaintances;
	}

	/**
	 * Returns true if two artists have a collaboration path connecting
	 * them in the database (see A3 handout for our definition of a path).
	 * For example, artists `Z' and `Usher' are considered connected even though
	 * they have not collaborated directly on any song, because 'Z' collaborated
	 * with `Alicia Keys' who in turn had collaborated with `Usher', therefore there
	 * is a collaboration path connecting `Z' and `Usher'.
	 *
	 * Returns false if there is no collaboration path at all between artist1 and artist2
	 * or if either of them do not exist in the database.
	 *
	 * @return    true iff artist1 and artist2 have a collaboration path connecting them
	 */
	public boolean artistConnectivity(String artist1, String artist2) {

		// see if artists exist
		Integer A1ID = artistExists(artist1);
		Integer A2ID = artistExists(artist2);
		if (A1ID == null || A2ID == null){ // either artist does not exist
			return false;
		}

		if (A1ID.equals(A2ID)){ // same artists provided
			return false;
		}

		// perform BFS on all collaborators of artist1 to find path to artist2
		Queue<String> fringe = new LinkedList<>();
		Set<String> explored = new HashSet<>();

		fringe.add(artist1);
		explored.add(artist1);

		while (!fringe.isEmpty()) { // while an artist is in the fringe
			String artist = fringe.poll();

			ArrayList<String> artistCollabs = findCollaborators(artist);

			for (String collaborator : artistCollabs) { 
				if (collaborator.equals(artist2)) { // path found
					return true;
				}
				if (!explored.contains(collaborator)) {
					fringe.add(collaborator);
					explored.add(collaborator);

				}
			}
		}


		return false; // temporarily
	}


	public static void main(String[] args) {

		if( args.length < 2 ){
			System.out.println("Usage: java ArtistBrowser <userName> <password>");
			return;
		}

		String user = args[0];
		String pass = args[1];

		ArtistBrowser a3 = new ArtistBrowser();

		String url = "jdbc:postgresql://localhost:5432/postgres?currentSchema=artistDB";
		a3.connectDB(url, user, pass);

		System.err.println("\n----- ArtistsInBands -----");
    ArrayList<String> res = a3.findArtistsInBands(1990,1999);
    for (String s : res) {
      System.err.println(s);
    }

		System.err.println("\n----- ArtistsInGenre -----");
    res = a3.findArtistsInGenre("Rock");
    for (String s : res) {
      System.err.println(s);
    }

		System.err.println("\n----- Collaborators -----");
		res = a3.findCollaborators("Usher");
		for (String s : res) {
		  System.err.println(s);
		}

		System.err.println("\n----- Songwriters -----");
	        res = a3.findSongwriters("Justin Bieber");
		for (String s : res) {
		  System.err.println(s);
		}

		System.err.println("\n----- Common Acquaintances -----");
		res = a3.findCommonAcquaintances("Jaden Smith", "Miley Cyrus");
		for (String s : res) {
		  System.err.println(s);
		}

		System.err.println("\n----- artistConnectivity -----");
		String a1 = "Z", a2 = "Usher";
		boolean areConnected = a3.artistConnectivity(a1, a2);
		System.err.println("Do artists " + a1 + " and " + a2 + " have a collaboration path connecting them? Answer: " + areConnected);

		a3.disconnectDB();
	}

}
