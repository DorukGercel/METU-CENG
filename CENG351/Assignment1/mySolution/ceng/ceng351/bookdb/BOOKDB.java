package ceng.ceng351.bookdb;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class BOOKDB implements IBOOKDB{
    private static String user = "2310027"; // TODO: Your userName
    private static String password = "dd0740f5"; //  TODO: Your password
    private static String host = "144.122.71.65"; // host name
    private static String database = "db2310027"; // TODO: Your database name
    private static int port = 8084; // port

    private Connection con;

    public BOOKDB(){

    }

    public void initialize(){
        String url = "jdbc:mysql://" + this.host + ":" + this.port + "/" + this.database + "?autoReconnect=true&useSSL=false";

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            this.con =  DriverManager.getConnection(url, this.user, this.password);
        } catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    public int createAuthorTables(){
        int result;
        int numberofTablesInserted = 0;

        //author(author id:int, author name:varchar(60))
        String queryCreateAuthorTable = "create table author (" + 
                                                                   "author_id int," +
                                                                   "author_name varchar(60)," +
                                                                   "primary key(author_id)"+
                                                            ");";
        try {
            Statement statement = this.con.createStatement();

            //Author Table
            result = statement.executeUpdate(queryCreateAuthorTable);
            //System.out.println(result);
            numberofTablesInserted++;

            //close
            statement.close();
            return numberofTablesInserted;
        } catch (SQLException e) {
            e.printStackTrace();
            return numberofTablesInserted;
        }
    }

    public int createPublisherTables(){
        int result;
        int numberofTablesInserted = 0;

        //publisher(publisher id:int, publisher name:varchar(50))
        String queryCreatePublisherTable = "create table publisher (" + 
                                                                   "publisher_id int," +
                                                                   "publisher_name varchar(50)," +
                                                                   "primary key(publisher_id)"+
                                                            ");";
        try {
            Statement statement = this.con.createStatement();

            //Author Table
            result = statement.executeUpdate(queryCreatePublisherTable);
            //System.out.println(result);
            numberofTablesInserted++;

            //close
            statement.close();
            return numberofTablesInserted;
        } catch (SQLException e) {
            e.printStackTrace();
            return numberofTablesInserted;
        }
    }

    public int createBookTables(){
        int result;
        int numberofTablesInserted = 0;

        //book(isbn:char(13), book name:varchar(120), publisher id:int, first publish year:char(4), page count:int, category:varchar(25),
        //rating:float) REFERENCES publisher(publisher id)
        String queryCreateBookTable = "create table book (" + 
                                                                   "isbn char(13)," +
                                                                   "book_name varchar(120)," +
                                                                   "publisher_id int," +
                                                                   "first_publish_year char(4)," +
                                                                   "page_count int," +
                                                                   "category varchar(25)," +
                                                                   "rating float," +
                                                                   "primary key(isbn),"+
                                                                   "foreign key(publisher_id) references publisher(publisher_id)" + 

                                                            ");";
        try {
            Statement statement = this.con.createStatement();

            //Book Table
            result = statement.executeUpdate(queryCreateBookTable);
            //System.out.println(result);
            numberofTablesInserted++;

            //close
            statement.close();
            return numberofTablesInserted;
        } catch (SQLException e) {
            e.printStackTrace();
            return numberofTablesInserted;
        }
    }

    public int createAuthorOfTables(){
        int result;
        int numberofTablesInserted = 0;

        //author of (isbn:char(13),author id:int) REFERENCES book(isbn) author(author id)
        String queryCreateAuthorOfTable = "create table author_of (" + 
                                                                   "isbn char(13)," +
                                                                   "author_id int," +
                                                                   "primary key(isbn, author_id),"+
                                                                   "foreign key(isbn) references book(isbn)," +
                                                                   "foreign key(author_id) references author(author_id)" +
                                                            ");";
        try {
            Statement statement = this.con.createStatement();

            //AuthorOf Table
            result = statement.executeUpdate(queryCreateAuthorOfTable);
            //System.out.println(result);
            numberofTablesInserted++;

            //close
            statement.close();
            return numberofTablesInserted;
        } catch (SQLException e) {
            e.printStackTrace();
            return numberofTablesInserted;
        }
    }

    public int createPHW1(){
        int result;
        int numberofTablesInserted = 0;

        //phw1(isbn:char(13), book name:varchar(120), rating:float)
        String queryCreatePHW1 = "create table phw1 (" + 
                                                                   "isbn char(13)," +
                                                                   "book_name varchar(120)," +
                                                                   "rating float," +
                                                                   "primary key(isbn)"+
                                                            ");";
        try {
            Statement statement = this.con.createStatement();

            //AuthorOf Table
            result = statement.executeUpdate(queryCreatePHW1);
            //System.out.println(result);
            numberofTablesInserted++;

            //close
            statement.close();
            return numberofTablesInserted;
        } catch (SQLException e) {
            e.printStackTrace();
            return numberofTablesInserted;
        }
    }

    
    public int createTables() {
        int numberofTablesInserted =createAuthorTables() +
                                    createPublisherTables() +
                                    createBookTables() + 
                                    createAuthorOfTables() +
                                    createPHW1();
        return numberofTablesInserted;
    }

    public int dropAuthorTables(){
        int result;
        int numberofTablesDropped = 0;

        String queryDropTable = "drop table if exists author;";


        try {
            Statement statement = this.con.createStatement();

            result = statement.executeUpdate(queryDropTable);
            numberofTablesDropped++;
            //System.out.println(result);

            //close
            statement.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return numberofTablesDropped;
    }

    public int dropBookTables(){
        int result;
        int numberofTablesDropped = 0;

        String queryDropTable = "drop table if exists book;";


        try {
            Statement statement = this.con.createStatement();

            result = statement.executeUpdate(queryDropTable);
            numberofTablesDropped++;
            //System.out.println(result);

            //close
            statement.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return numberofTablesDropped;
    }

    public int dropPublisherTables(){
        int result;
        int numberofTablesDropped = 0;

        String queryDropTable = "drop table if exists publisher;";


        try {
            Statement statement = this.con.createStatement();

            result = statement.executeUpdate(queryDropTable);
            numberofTablesDropped++;
            //System.out.println(result);

            //close
            statement.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return numberofTablesDropped;
    }

    public int dropAuthorOfTables(){
        int result;
        int numberofTablesDropped = 0;

        String queryDropTable = "drop table if exists author_of;";


        try {
            Statement statement = this.con.createStatement();

            result = statement.executeUpdate(queryDropTable);
            numberofTablesDropped++;
            //System.out.println(result);

            //close
            statement.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return numberofTablesDropped;
    }

    public int dropPHW1Tables(){
        int result;
        int numberofTablesDropped = 0;

        String queryDropTable = "drop table if exists phw1;";


        try {
            Statement statement = this.con.createStatement();

            result = statement.executeUpdate(queryDropTable);
            numberofTablesDropped++;
            //System.out.println(result);

            //close
            statement.close();

        } catch (SQLException e) {
            e.printStackTrace();
        }
        return numberofTablesDropped;
    }

    public int dropTables(){
        int numberofTablesDropped = dropPHW1Tables() +
                                    dropAuthorOfTables() +
                                    dropBookTables() +
                                    dropAuthorTables() +
                                    dropPublisherTables();
        return numberofTablesDropped;
    }

    /**
     * Should insert an array of Author into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertOneAuthor(Author author){
        int result = 0;

        String query = "insert into author values (\"" + 
                        author.getAuthor_id() + "\",\"" + 
                        author.getAuthor_name() 
                        + "\");";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            //System.out.println(result);

            st.close();
            return 1;

        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        }
    }

    public int insertAuthor(Author[] authors){
        int result = 0;
        for(int i = 0; i < authors.length; i++){
            result+=insertOneAuthor(authors[i]);
        }
        return result;
    }

    /**
     * Should insert an array of Book into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertOneBook(Book book){
        int result = 0;

        String query = "insert into book values (\"" + 
                        book.getIsbn() + "\",\"" +
                        book.getBook_name() + "\",\"" +
                        book.getPublisher_id() + "\",\"" +
                        book.getFirst_publish_year() + "\",\"" +
                        book.getPage_count() + "\",\"" +
                        book.getCategory() + "\",\"" +
                        book.getRating()
                        + "\");";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            //System.out.println(result);

            st.close();
            return 1;

        } catch (SQLException e) {
            //System.out.println(book.getIsbn());
            //System.out.println(book.getBook_name());
            e.printStackTrace();
            return 0;
        }
    }

    public int insertBook(Book[] books){
        int result = 0;
        for(int i = 0; i < books.length; i++){
            result+=insertOneBook(books[i]);
            
        }
        return result;
    }

    /**
     * Should insert an array of Publisher into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertOnePublisher(Publisher publisher){
        int result = 0;

        String query = "insert into publisher values (\"" + 
                        publisher.getPublisher_id() + "\",\"" +
                        publisher.getPublisher_name()
                        + "\");";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            //System.out.println(result);

            st.close();
            return 1;

        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        }
    }

    public int insertPublisher(Publisher[] publishers){
        int result = 0;
        for(int i = 0; i < publishers.length; i++){
            result+=insertOnePublisher(publishers[i]);
        }
        return result;
    }

    /**
     * Should insert an array of Author_of into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertOneAuthor_of(Author_of author_of){
        int result = 0;

        String query = "insert into author_of values (\"" + 
                        author_of.getIsbn() + "\",\"" +
                        author_of.getAuthor_id()
                        + "\");";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            //System.out.println(result);

            st.close();
            return 1;

        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        }
    }

    public int insertAuthor_of(Author_of[] author_ofs){
        int result = 0;
        for(int i = 0; i < author_ofs.length; i++){
            result+=insertOneAuthor_of(author_ofs[i]);
        }
        return result;
    }

    /**
     * Should return isbn, first_publish_year, page_count and publisher_name of 
     * the books which have the maximum number of pages.
     * @return QueryResult.ResultQ1[]
     */
    public QueryResult.ResultQ1[] functionQ1(){
        ResultSet rs;
        QueryResult.ResultQ1[] resultQ1 = null;

        String query = "select isbn, first_publish_year, page_count, publisher_name "+
                        "from book B, publisher P " +
                        "where B.publisher_id = P.publisher_id and B.page_count >= " +
                            "(select max(page_count) " + 
                            "from book) " +
                        "order by B.isbn asc;";

        try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            
            int size = 0;
            if(rs != null){
                rs.last();
                size = rs.getRow();
                rs.beforeFirst();
            }

            resultQ1 = new QueryResult.ResultQ1[size];

            for(int i = 0; i < size; i++){
                rs.next();

                String isbn = rs.getString("isbn");
                String first_publish_year = rs.getString("first_publish_year");
                int page_count = rs.getInt("page_count");
                String publisher_name = rs.getString("publisher_name");

                resultQ1[i] = new QueryResult.ResultQ1(isbn, first_publish_year, page_count, publisher_name);
            }
            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return resultQ1;
    }
    
    /**
     * For the publishers who have published books that were co-authored by both 
     * of the given authors(author1 and author2); list publisher_id(s) and average
     * page_count(s)  of all the books these publishers have published.
     * @param author_id1
     * @param author_id2
     * @return QueryResult.ResultQ2[]
     */

    public QueryResult.ResultQ2[] functionQ2(int author_id1, int author_id2){
        ResultSet rs;
        QueryResult.ResultQ2[] resultQ2 = null;

        String query = "SELECT publisher_id, avg(page_count) as average_page_count " +
                       "FROM book " +
                       "WHERE publisher_id IN " +
                       "(SELECT B.publisher_id " +
                       "FROM book B, author_of A1, author_of A2 " +
                       "WHERE A1.author_id = " + author_id1 + " and A2.author_id = " + author_id2 + " and A1.isbn = A2.isbn and A1.isbn = B.isbn and A2.isbn = B.isbn) " +
                       "GROUP BY publisher_id " +
                       "order by publisher_id asc; ";


        try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            
            int size = 0;
            if(rs != null){
                rs.last();
                size = rs.getRow();
                rs.beforeFirst();
            }

            resultQ2 = new QueryResult.ResultQ2[size];

            for(int i = 0; i < size; i++){
                rs.next();

                int publisher_id = rs.getInt("publisher_id");
                double average_page_count = rs.getDouble("average_page_count");

                resultQ2[i] = new QueryResult.ResultQ2(publisher_id, average_page_count);
            }
            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return resultQ2;
    }
    
    /**
     * List book_name, category and first_publish_year of the earliest 
     * published book(s) of the author(s) whose author_name is given.
     * @param author_name
     * @return QueryResult.ResultQ3[]
     */

    public QueryResult.ResultQ3[] functionQ3(String author_name){
        ResultSet rs;
        QueryResult.ResultQ3[] resultQ3 = null;

        String query = "SELECT book_name, category, first_publish_year " +
                       "FROM ( " +
                       "    SELECT B.book_name, B.category, B.first_publish_year " +
                       "    FROM book B, author_of AO, author A  " +
                       "    WHERE A.author_name = \""+ author_name + "\" and A.author_id = AO.author_id and AO.isbn = B.isbn " +
                       ") as temp " +
                       "WHERE temp.first_publish_year <= ( " +
                       "        SELECT B.first_publish_year " +
                       "        FROM book B, author_of AO, author A  " +
                       "        WHERE A.author_name = \"" + author_name + "\" and A.author_id = AO.author_id and AO.isbn = B.isbn " +
                       ") " +
                       "order by book_name, category, first_publish_year asc;  ";


        try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            
            int size = 0;
            if(rs != null){
                rs.last();
                size = rs.getRow();
                rs.beforeFirst();
            }

            resultQ3 = new QueryResult.ResultQ3[size];

            for(int i = 0; i < size; i++){
                rs.next();

                String book_name = rs.getString("book_name");
                String category = rs.getString("category");
                String first_publish_year = rs.getString("first_publish_year");

                resultQ3[i] = new QueryResult.ResultQ3(book_name, category, first_publish_year);
            }
            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return resultQ3;
    }

    /**
     * For publishers whose name contains at least 3 words 
     * (i.e., "Koc Universitesi Yayinlari"), have published at least 3 books 
     * and average rating of their books are greater than(>) 3; 
     * list their publisher_id(s) and distinct category(ies) they have published. 
     * PS: You may assume that each word in publisher_name is seperated by a space.
     * @return QueryResult.ResultQ4[]
     */
    public QueryResult.ResultQ4[] functionQ4(){
        ResultSet rs;
        QueryResult.ResultQ4[] resultQ4 = null;

        String query = "SELECT DISTINCT tmp.publisher_id as publisher_id, B.category as category " +
                       "FROM( " +
                       "    SELECT B.publisher_id " +
                       "    FROM book B, publisher P  " +
                       "    WHERE B.publisher_id = P.publisher_id and P.publisher_name LIKE \"% % %\" " +
                       "    GROUP BY B.publisher_id " +
                       "    HAVING COUNT(*) >= 3 and AVG(B.rating) > 3 " +
                       ")AS tmp, book B " +
                       "WHERE tmp.publisher_id = B.publisher_id " +
                       "ORDER BY tmp.publisher_id, B.category;  ";


        try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            
            int size = 0;
            if(rs != null){
                rs.last();
                size = rs.getRow();
                rs.beforeFirst();
            }

            resultQ4 = new QueryResult.ResultQ4[size];

            for(int i = 0; i < size; i++){
                rs.next();

                int publisher_id = rs.getInt("publisher_id");
                String category = rs.getString("category");

                resultQ4[i] = new QueryResult.ResultQ4(publisher_id , category);
            }
            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return resultQ4;
    }
    
    /**
     * List author_id and author_name of the authors who have worked with 
     * all the publishers that the given author_id has worked.
     * @param author_id
     * @return QueryResult.ResultQ5[]
     */

    public QueryResult.ResultQ5[] functionQ5(int author_id){
        ResultSet rs;
        QueryResult.ResultQ5[] resultQ5 = null;

        String query = "SELECT A.author_id, A.author_name " +
                       "FROM author A " +
                       "WHERE NOT EXISTS( " +
                       "        SELECT B.publisher_id " +
                       "        FROM author_of AO, book B, publisher P " +
                       "        WHERE AO.author_id = " + author_id + " and AO.isbn = B.isbn and B.publisher_id = P.publisher_id and P.publisher_id NOT IN( " +
                       "            SELECT b.publisher_id " +
                       "            FROM author_of ao, book b " +
                       "            WHERE ao.author_id = A.author_id and ao.isbn = b.isbn " +
                       "        )    " +
                       ") " +
                       "order by author_id asc; ";


        try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            
            int size = 0;
            if(rs != null){
                rs.last();
                size = rs.getRow();
                rs.beforeFirst();
            }

            resultQ5 = new QueryResult.ResultQ5[size];

            for(int i = 0; i < size; i++){
                rs.next();

                author_id = rs.getInt("author_id");
                String author_name = rs.getString("author_name");

                resultQ5[i] = new QueryResult.ResultQ5(author_id , author_name);
            }
            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return resultQ5;
    }
    
    /**
     * List author_name(s) and isbn(s) of the book(s) written by "Selective" authors. 
     * "Selective" authors are those who have worked with publishers that have 
     * published their books only.(i.e., they haven't published books of 
     * different authors)
     * @return QueryResult.ResultQ6[]
     */

    public QueryResult.ResultQ6[] functionQ6(){
        ResultSet rs;
        QueryResult.ResultQ6[] resultQ6 = null;

        String query = "SELECT DISTINCT author.author_id, author_of.isbn " +
                       "FROM author , author_of " +
                       "WHERE author.author_id = author_of.author_id and author.author_id NOT IN ( " +
                       "    SELECT AO1.author_id " +
                       "    FROM author_of AO1, author_of AO2, book B1, book B2 " +
                       "    WHERE AO1.isbn = B1.isbn and AO2.isbn = B2.isbn and B1.publisher_id = B2.publisher_id and AO1.author_id != AO2.author_id " +
                       ") " +
                       "order by author.author_id, author_of.isbn asc; ";


        try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            
            int size = 0;
            if(rs != null){
                rs.last();
                size = rs.getRow();
                rs.beforeFirst();
            }

            resultQ6 = new QueryResult.ResultQ6[size];

            for(int i = 0; i < size; i++){
                rs.next();

                int author_id = rs.getInt("author_id");
                String isbn = rs.getString("isbn");

                resultQ6[i] = new QueryResult.ResultQ6(author_id, isbn);
            }
            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return resultQ6;
    }

    /**
     * List publisher_id and publisher_name of the publishers who have published 
     * at least 2 books in  'Roman' category and average rating of their books 
     * are more than (>) the given value.
     * @param rating
     * @return QueryResult.ResultQ7[]
     */
    public QueryResult.ResultQ7[] functionQ7(double rating){
        ResultSet rs;
        QueryResult.ResultQ7[] resultQ7 = null;

        String query = "SELECT P.publisher_id, P.publisher_name " +
                       "FROM ( " +
                       "    SELECT B.publisher_id " +
                       "    FROM book B  " +
                       "    WHERE B.category = \"Roman\" " +
                       "    GROUP BY B.publisher_id " +
                       "    HAVING COUNT(*) >= 2 " +
                       ") as tmp, book B, publisher P  " +
                       "WHERE tmp.publisher_id = P.publisher_id and tmp.publisher_id = B.publisher_id " +
                       "GROUP BY B.publisher_id " +
                       "HAVING AVG(B.rating) > " + rating +
                       "order by P.publisher_id; ";


        try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            
            int size = 0;
            if(rs != null){
                rs.last();
                size = rs.getRow();
                rs.beforeFirst();
            }

            resultQ7 = new QueryResult.ResultQ7[size];

            for(int i = 0; i < size; i++){
                rs.next();

                int publisher_id = rs.getInt("publisher_id");
                String publisher_name = rs.getString("publisher_name");

                resultQ7[i] = new QueryResult.ResultQ7(publisher_id, publisher_name);
            }
            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return resultQ7;
    }
    
    /**
     * Some of the books  in the store have been published more than once: 
     * although they have same names(book\_name), they are published with different
     * isbns. For each  multiple copy of these books, find the book_name(s) with the 
     * lowest rating for each book_name  and store their isbn, book_name and 
     * rating into phw1 table using a single BULK insertion query. 
     * If there exists more than 1 with the lowest rating, then store them all.
     * After the bulk insertion operation, list isbn, book_name and rating of 
     * all rows in phw1 table.
     * @return QueryResult.ResultQ8[]
     */

    public void insertQ8(){
        int result = 0;

        String query = "INSERT INTO phw1 " +
                       "SELECT DISTINCT B1_isbn as isbn, B1_name as book_name, B1_rating as rating " +
                       "FROM ( " +
                       "    SELECT B1.book_name as B1_name, B1.isbn as B1_isbn, B1.rating as B1_rating, B2.book_name as B2_name, B2.isbn as B2_isbn, B2.rating as B2_rating   " +
                       "    FROM book B1, book B2 " +
                       "    WHERE B1.book_name = B2.book_name  and B1.isbn != B2.isbn  " +
                       ") as tmp " +
                       "where tmp.B1_isbn NOT IN( " +
                       "    SELECT tmp.B1_isbn " +
                       "    FROM ( " +
                       "        SELECT B1.book_name as B1_name, B1.isbn as B1_isbn, B1.rating as B1_rating, B2.book_name as B2_name, B2.isbn as B2_isbn, B2.rating as B2_rating   " +
                       "        FROM book B1, book B2 " +
                       "        WHERE B1.book_name = B2.book_name) as tmp " +
                       "    WHERE tmp.B1_isbn > tmp.B2_isbn " +
                       "); ";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            //System.out.println(result);

            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    public QueryResult.ResultQ8[] functionQ8(){
        ResultSet rs;
        QueryResult.ResultQ8[] resultQ8 = null;

        insertQ8();
        String query = "SELECT isbn, book_name, rating " +
                       "FROM phw1 " +
                       "order by isbn asc; ";


        try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            
            int size = 0;
            if(rs != null){
                rs.last();
                size = rs.getRow();
                rs.beforeFirst();
            }

            resultQ8 = new QueryResult.ResultQ8[size];

            for(int i = 0; i < size; i++){
                rs.next();

                String isbn = rs.getString("isbn");
                String book_name = rs.getString("book_name");
                double rating = rs.getDouble("rating");

                resultQ8[i] = new QueryResult.ResultQ8(isbn, book_name, rating);
            }
            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return resultQ8;
    }
    
    /**
     * For the books that contain the given keyword anywhere in their names, 
     * increase their ratings by one. 
     * Please note that, the maximum rating cannot be more than 5, 
     * therefore if the previous rating is greater than 4, do not update the 
     * rating of that book. 
     * @param keyword
     * @return sum of the ratings of all books
     */

    public void updateQ9(String keyword){
        int result = 0;

        String query = "UPDATE book  " +
                       "SET book.rating = book.rating + 1 " +
                       "WHERE book.book_name LIKE \"%" + keyword + "%\" AND book.rating <=4; ";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            //System.out.println(result);

            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    public double functionQ9(String keyword){
        ResultSet rs;
        double sum_rating = 0.0;

        updateQ9(keyword);
        String query = "SELECT SUM(rating) as sum_rating " +
                       "FROM book; ";

        try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            
            int size = 0;
            if(rs != null){
                rs.last();
                size = rs.getRow();
                rs.beforeFirst();
            }

            for(int i = 0; i < size; i++){
                rs.next();
                sum_rating = rs.getDouble("sum_rating");

            }
            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return sum_rating;
    }
    
    /**
     * Delete publishers in publisher table who haven't published a book yet. 
     * @return count of all rows of the publisher table after delete operation.
     */
    public void deleteQ10(){
        int result = 0;

        String query = "DELETE FROM publisher " + 
                       "WHERE publisher.publisher_id NOT IN( " + 
                       "    SELECT publisher_id " + 
                       "    FROM book  " + 
                       "); ";

        try {
            Statement st = this.con.createStatement();
            result = st.executeUpdate(query);
            //System.out.println(result);

            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    public int function10(){
        ResultSet rs;
        int count = 0;

        deleteQ10();;
        String query = "SELECT COUNT(*) as count " + 
                       "FROM publisher; ";

        try {
            Statement st = this.con.createStatement();
            rs = st.executeQuery(query);
            
            int size = 0;
            if(rs != null){
                rs.last();
                size = rs.getRow();
                rs.beforeFirst();
            }

            for(int i = 0; i < size; i++){
                rs.next();
                count = rs.getInt("count");

            }
            //Close
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return count;
    }

    
}