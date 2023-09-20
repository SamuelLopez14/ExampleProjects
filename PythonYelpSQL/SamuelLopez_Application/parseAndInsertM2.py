import json
import psycopg2

def cleanStr4SQL(s):
    return s.replace("'","`").replace("\n"," ")

def int2BoolStr (value):
    if value == 0:
        return 'False'
    else:
        return 'True'

def insert2BusinessTable():
    #reading the JSON file
    with open('E:\CPTS451\Milestone 2\Yelp-CptS451/yelp_business.JSON','r') as f:    #TODO: update path for the input file
        #outfile =  open('.//yelp_dataset//yelp_business.SQL', 'w')  #uncomment this line if you are writing the INSERT statements to an output file.
        line = f.readline()
        count_line = 0

        #connect to yelpdb database on postgres server using psycopg2
        #TODO: update the database name, username, and password
        try:
            conn = psycopg2.connect("dbname='yelpdb_temp' user='postgres' host='localhost' password='12345'")
        except:
            print('Unable to connect to the database!')
        cur = conn.cursor()

        while line:
            data = json.loads(line)
            #Generate the INSERT statement for the cussent business
            #TODO: The below INSERT statement is based on a simple (and incomplete) businesstable schema. Update the statment based on your own table schema ans
            # include values for all businessTable attributes
            sql_str = "INSERT INTO business (BusinessID, name, Address,State,City,Zip,reviewrating,Revew_count,numCheckins) " + \
                      "VALUES ('" + cleanStr4SQL(data['business_id']) + "','" + cleanStr4SQL(data['name']) + "','" + cleanStr4SQL(data["address"]) + "','" + \
                      cleanStr4SQL(data["state"]) + "','" + cleanStr4SQL(data["city"]) + "','" + cleanStr4SQL(data["postal_code"]) + "',0,0,0);"
            try:
                cur.execute(sql_str)
            except:
                print(sql_str)
                print("Insert to businessTABLE failed!")
            conn.commit()

            #Insert Categories into database
            business = cleanStr4SQL(data['business_id'])
            for category in data['categories']:
                category_str = "INSERT INTO BusinessCategory (BusinessID, category) " + \
                            "VALUES ('" + business + "','" + cleanStr4SQL(category) +"');"
                #print(category_str)
                try:
                    cur.execute(category_str)
                except:
                    print(category_str)
                    print("Insert to BusinessCategory failed!")
            
            conn.commit()
            # optionally you might write the INSERT statement to a file.
            # outfile.write(sql_str)

            line = f.readline()
            count_line +=1

        cur.close()
        conn.close()

    print(count_line)
    #outfile.close()  #uncomment this line if you are writing the INSERT statements to an output file.
    f.close()

def insert2UserTable():
    print("Starting insert from  user table")
    #reading the JSON file
    with open('E:\CPTS451\Milestone 2\Yelp-CptS451/yelp_user.JSON','r') as f:    #TODO: update path for the input file
        #outfile =  open('.//yelp_dataset//yelp_business.SQL', 'w')  #uncomment this line if you are writing the INSERT statements to an output file.
        line = f.readline()
        count_line = 0

        #connect to yelpdb database on postgres server using psycopg2
        #TODO: update the database name, username, and password
        try:
            conn = psycopg2.connect("dbname='yelpdb_temp' user='postgres' host='localhost' password='12345'")
        except:
            print('Unable to connect to the database!')
        cur = conn.cursor()

        while line:
            data = json.loads(line)
            #Generate the INSERT statement for the cussent business
            #TODO: The below INSERT statement is based on a simple (and incomplete) businesstable schema. Update the statment based on your own table schema ans
            # include values for all businessTable attributes
            sql_str = "INSERT INTO Users (UserID,Since,Average_Stars,Name)" + \
                      "VALUES ('" + cleanStr4SQL(data['user_id']) + "', TO_DATE('" + data["yelping_since"] + "' ,'YYY/MM/DD') ," + str(data["average_stars"]) + ", '" + cleanStr4SQL(data['name']) +"')"
            try:
                cur.execute(sql_str)
            except:
                print(sql_str)
                print("Insert to Users TABLE failed!")
            conn.commit()



            line = f.readline()
            count_line +=1

        cur.close()
        conn.close()

    print(count_line)
    #outfile.close()  #uncomment this line if you are writing the INSERT statements to an output file.
    f.close()

def insert2CheckinTable():
    print("Starting insert from  user table")
    #reading the JSON file
    with open('E:\CPTS451\Milestone 2\Yelp-CptS451/yelp_checkin.JSON','r') as f:    #TODO: update path for the input file
        #outfile =  open('.//yelp_dataset//yelp_business.SQL', 'w')  #uncomment this line if you are writing the INSERT statements to an output file.
        line = f.readline()
        count_line = 0

        #connect to yelpdb database on postgres server using psycopg2
        #TODO: update the database name, username, and password
        try:
            conn = psycopg2.connect("dbname='yelpdb_temp' user='postgres' host='localhost' password='12345'")
        except:
            print('Unable to connect to the database!')
        cur = conn.cursor()

        while line:
            data = json.loads(line)
            #Generate the INSERT statement for the cussent business
            #TODO: The below INSERT statement is based on a simple (and incomplete) businesstable schema. Update the statment based on your own table schema ans
            # include values for all businessTable attributes

            for (dayofweek,time) in data['time'].items():
                for (hour,count) in time.items():

                    sql_str = "INSERT INTO Checkins (Time,BusinessID,NumCheckins,Day)" + \
                      "VALUES ('" + hour + "', '" + cleanStr4SQL(data['business_id']) + "' ," + str(count) + ", '" + dayofweek +"')"
                    try:
                        cur.execute(sql_str)
                    except:
                        print(sql_str)
                        print("Insert to Checkins TABLE failed!")
                    conn.commit()



            line = f.readline()
            count_line +=1

        cur.close()
        conn.close()

    print(count_line)
    #outfile.close()  #uncomment this line if you are writing the INSERT statements to an output file.
    f.close()

def insert2ReviewTable():
    print("Starting insert from  Review table")
    #reading the JSON file
    with open('E:\CPTS451\Milestone 2\Yelp-CptS451/yelp_review.JSON','r') as f:    #TODO: update path for the input file
        #outfile =  open('.//yelp_dataset//yelp_business.SQL', 'w')  #uncomment this line if you are writing the INSERT statements to an output file.
        line = f.readline()
        count_line = 0

        #connect to yelpdb database on postgres server using psycopg2
        #TODO: update the database name, username, and password
        try:
            conn = psycopg2.connect("dbname='yelpdb_temp' user='postgres' host='localhost' password='12345'")
        except:
            print('Unable to connect to the database!')
        cur = conn.cursor()

        while line:
            data = json.loads(line)
            #Generate the INSERT statement for the cussent business
            #TODO: The below INSERT statement is based on a simple (and incomplete) businesstable schema. Update the statment based on your own table schema ans
            # include values for all businessTable attributes
            review_str = "INSERT INTO Review (ReviewID,date,stars,text)" + \
                      "VALUES ('" + cleanStr4SQL(data['review_id']) + "', TO_DATE('" + data['date'] + "' ,'YYY/MM/DD') ," + str(data['stars']) + ", '" + cleanStr4SQL(data['text']) +"')"
            try:
                cur.execute(review_str)
            except:
                print(review_str)
                print("Insert to Review TABLE failed!")
            conn.commit()
            
            reviews_str = "INSERT INTO reviews (ReviewID,BusinessID,UserID)" + \
                      "VALUES ('" + cleanStr4SQL(data['review_id']) + "','" + cleanStr4SQL(data['business_id']) + "','" + cleanStr4SQL(data['user_id']) +"')"
            try:
                cur.execute(reviews_str)
            except:
                print(reviews_str)
                print("Insert to ReviewSSSS TABLE failed!")
            conn.commit()



            line = f.readline()
            count_line +=1

        cur.close()
        conn.close()

    print(count_line)
    #outfile.close()  #uncomment this line if you are writing the INSERT statements to an output file.
    f.close()

#insert2BusinessTable()
#insert2UserTable()
#insert2CheckinTable()
#insert2ReviewTable()