from sanic import Sanic

from users import users
from etc import etc

app = Sanic()
app.blueprint(users)
app.blueprint(etc)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080)
