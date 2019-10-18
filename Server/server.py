import socket
import pymysql

class Sql(object):
    def __init__(self):
        try:
            self.connect = pymysql.connect(host="127.0.0.1", port=3306,
                user="root", password="niw123", db="virus4secguard")
            # 通过连接对象获取到游标对象
            self.cursor = self.connect.cursor()
        except Exception as e:
            # 打印错误信息
            print("Error:", e)

    # 用于执行添加操作的语句
    def execute(self, sql):
        try:
            # 应该使用异常处理打印信息
            self.cursor.execute(sql)
            # 所有修改了数据库内容的语句都需要提交
            self.connect.commit()
        except Exception as e:
            # 打印错误信息
            print("Error:", e)
            # 如果产生了错误，那么可能会影响到数据库
            # 如果不想让指令影响数据库，应该回滚
            self.connect.rollback()

    # 用于查找数据库内的信息
    def select(self, sql):
        try:
            # 应该使用异常处理打印信息
            self.cursor.execute(sql)
            # 获取到返回的结果，返回的是一个元组，元组的每一
            # 个元素又是一个元组，保存的是查询到的所有信息
            result = self.cursor.fetchall()
            # 需要获取查询的内容和条目
            return self.cursor.rowcount, result
        except Exception as e:
            # 打印错误信息
            print("Error:", e)

def main():
    # 创建服务器对象
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(("127.0.0.1", 6666))
    server.listen(socket.SOMAXCONN)
    # 创建数据库对象
    sql = Sql()
    while True:
        client, addr = server.accept()

        buff_md5 = client.recv(1000).decode('gb2312')
        sqlstr = "SELECT * FROM VirusInfo where md5 = \"%s\" ;" % buff_md5
        count, context = sql.select(sqlstr)
        if count==1:
            client.send("1".encode('gb2312'))
        else:
            client.send("0".encode('gb2312'))

        print(buff_md5)
        print(sqlstr)
        print(count, context)

if __name__ == "__main__":
    main()
