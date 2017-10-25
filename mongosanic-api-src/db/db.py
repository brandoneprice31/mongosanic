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
            'last_name': user['last_name'],
            'fb_token': user['fb_token'],
            'easy_highscores': [],
            'hard_highscores': []
        }

        return self.deserialize(self.db['users'].insert(user))

    def findUserByFbToken(self, fbToken):
        return self.deserialize(self.db['users'].find_one({ 'fb_token': fbToken }))

    def findUserById(self, userId):
        return self.deserialize(self.db['users'].find_one({ '_id': ObjectId(userId) }))

    def updateHighScores(self, userId, easyHS, hardHS):
        return self.db['users'].update_one({ '_id' : ObjectId(userId) }, { '$set': { 'easy_highscores': easyHS, 'hard_highscores': hardHS } } )

    def getAllUsersByFBTokens(self, fbTokens):
        return self.deserialize(list(self.db['users'].find({ 'fb_token': { '$in' : fbTokens } })))

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
