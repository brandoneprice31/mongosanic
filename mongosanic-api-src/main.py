from sanic import Sanic

app = Sanic()
app.blueprint(users)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8081)
