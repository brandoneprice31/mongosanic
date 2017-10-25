import json
import datetime
from pymongo import MongoClient
from bson import ObjectId

from config.config import Config


class DB:

    def __init__(self):
        self.config = Config()
        self.db = MongoClient(self.config.dbURI)[self.config.dbName]

    def insertUser(self, user):
        user = {
            'first_name': user['first_name'],
            'last_name': user['last_name']
        }

        return self.deserialize(self.db['users'].insert(user))

    def findUserById(self, userId):
        return self.deserialize(self.db['users'].find_one({ '_id': ObjectId(userId) }))

    # Helpers

    def deserialize(self, object):
        return json.loads(JSONEncoder().encode(object))


class JSONEncoder(json.JSONEncoder):
    def default(self, o):
        if isinstance(o, ObjectId):
            return str(o)
        elif isinstance(o, (datetime.datetime, datetime.date)):
            return o.isoformat()
        return json.JSONEncoder.default(self, o)


db = DB()
