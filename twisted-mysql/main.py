from twisted.enterprise import adbapi


# MySQL
dbpool = adbapi.ConnectionPool("pymysql", db="test")


# equivalent of cursor.execute(statement), return cursor.fetchall():
def getAge(user):
    return dbpool.runQuery("SELECT age FROM users WHERE name = ?", user)

def printResult(l):
    if l:
        print(l[0][0], "years old")
    else:
        print("No such user")

getAge("joe").addCallback(printResult)

print("end")

