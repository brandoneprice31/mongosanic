import asyncio
from sanic.response import json as json_response
from sanic import Blueprint

from db.db import db
from responses.response import Response

users = Blueprint('users')
baseURI = '/' + users.name

#
# POST - /users
# {
#   first_name: string,
#   last_name: string
# }
#
@users.route(baseURI, methods=['POST'])
async def postUser(request):
    body = request.json

    if 'first_name' not in body or 'last_name' not in body:
        return json_response({ 'error': Response.BadRequest }, status=400)

    user_id = db.insertUser(body)
    user = db.findUserById(user_id)

    return json_response({ 'user': user }, status=201)

#
# GET - /users/:user_id
#
@users.route(baseURI + '/<user_id>', methods=['GET'])
async def getUser(request, user_id):
    user = db.findUserById(user_id)
    if user == None:
        return json_response({ 'error': Response.NotFoundError }, status=404)

    return json_response({ 'user': user }, status=200)
