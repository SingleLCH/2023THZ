import matplotlib.pyplot as plt
import pymysql
import argparse
from DBUtils.PooledDB import PooledDB
from scipy.interpolate import griddata
import time
import datetime
import os
import base64

DBNAME = 'THZ'
DBTABLENAME = 'thz'
LastProcessTime = datetime.datetime(2020,1,1,0,0,0)
findLastChangeTime = "SELECT UPDATE_TIME FROM information_schema.tables WHERE TABLE_SCHEMA = '" + DBNAME + "' AND TABLE_NAME = '" + DBTABLENAME + "'"
findData = "SELECT * FROM " + DBNAME + "." + DBTABLENAME
insertPicture = "INSERT INTO my_images (name, image) VALUES (%s, %s)"

parser = argparse.ArgumentParser(description="mysql")
parser.add_argument("-u","--user",type=str,help="MsSQL Username",default = 'root')
parser.add_argument("-p","--password",type=str,help="MsSQL Password",default='Wsad080874')
parser.add_argument("-i","--hostip",type=str,help="remote/local ip",default='localhost')
parser.add_argument("-o","--port",type=int,help="MySQL port",default=3306)

i = 1

args = parser.parse_args()

DBUSER = args.user
DBPASS = args.password
DBHOST = args.hostip
DBPORT = args.port


pool = PooledDB(
    creator = pymysql,
    host = DBHOST,
    user = DBUSER,
    password = DBPASS,
    database = DBNAME,
    port = DBPORT
)


def connectServer():
    try:
        conn = pool.connection()
        cursor = conn.cursor()
        print("Connect to :",DBHOST,"Success.")
        return conn,cursor
    except Exception as e:
        print("Failed to connect database: ", e)
        exit()

def poll(conn, cursor):
    global LastProcessTime
    savenum = 0
    while True:
        cursor.execute(findLastChangeTime)
        gettime = cursor.fetchall()
        if gettime is None:
            print("Not find change time! Something Wrong?")
            break
        LastChangeTime = gettime[0][0]
        print("Get Table Last Change Time :",LastChangeTime)
        if LastProcessTime >= LastChangeTime:
            #wait for change
            time.sleep(10)
            continue
        else:
            #find data
            print("Find New Data!\nLastProcessTime is :",LastProcessTime,"    LastChangeTime is :",LastChangeTime)
            #Make sure to write complete
            time.sleep(5)
            x = []
            y = []
            z = []
            
            cursor.execute(findData)
            sqlData = cursor.fetchall()
            for i in range(len(sqlData)):
                if (sqlData[i][0] == None or sqlData[i][1] == None or sqlData[i][2] == None):
                    print("SQLData have none value, wtf!")
                    break
                x.append(-float(sqlData[i][0]))
                y.append(-float(sqlData[i][1]))
                z.append(-float(sqlData[i][2]))
                #print('x: %f, y: %f, z: %f' %(x[i], y[i], z[i]))
            print("Check data done,now process it.")
            try:
                plt.scatter(x, y, c=z, cmap='viridis')
                filename = str(datetime.datetime.now()) + ".png"
                path = "/root/SaveFile/" + filename
                print("Now time is: ",filename,"  Saving file to :", path)
                plt.savefig(path)
            except Exception as e:
                print("Process data failed: ", e)
                break
            #if process success############
            savenum+=1
            print("Save to local succuss,now insert to database")
            # Read image data
            try:
                with open(path, 'rb') as f:
                    img_data = f.read()
                    # Encode image data as base64 string
                    img_base64 = base64.b64encode(img_data).decode('utf-8')
                    # Insert image data into database
                    cursor.execute(insertPicture, (filename, img_base64))
                    conn.commit()
            except Exception as e:
                print("Insert to database failed: ", e)
                break
            LastProcessTime = datetime.datetime.now()
            print("Insert to database Success! Update LastProcessTime to : %s This is %dth time to save file."%(LastProcessTime, savenum))
        time.sleep(10)


if __name__ =='__main__':
    conn,cursor = connectServer()
    poll(conn,cursor)
    conn.close()    
    exit()

    



