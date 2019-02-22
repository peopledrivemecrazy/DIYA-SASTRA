#!/usr/bin/env python

import urllib2
import json
import os

from flask import Flask
from flask import request
from flask import make_response

# Flask app should start in global layout
app = Flask(__name__)


@app.route('/webhook', methods=['POST'])
def webhook():
    req = request.get_json(silent=True, force=True)

    print("Request:")
    print(json.dumps(req, indent=4))

    res = makeWebhookResult(req)

    res = json.dumps(res, indent=4)
    print(res)
    r = make_response(res)
    r.headers['Content-Type'] = 'application/json'
    return r

def makeWebhookResult(req):
    print req.get("result")
    if req.get("result").get("metadata").get("intentName") == "direction":
        #http://192.168.1.10/forward
        direction = req.get("result").get("parameters").get("direction")
        print direction
        steps = req.get("result").get("parameters").get("number")
        print steps
        #http://192.168.1.11/1?a=0&b=0
        currenturl = 'http://192.168.1.11/direction?d=' + str(direction) + '&s=' + str(steps)
        print currenturl
        response = urllib2.urlopen(currenturl)
        data = response.read()
        response.close()
        speech = "Moving " + str(direction) + " for " + str(steps)

        print("Response:")
        print(speech)
    
    elif req.get("result").get("metadata").get("intentName") == "temperature":
        #https://api.thingspeak.com/channels/708409/fields/1/last.json?api_key=OQON3Q83OX2G54HT
      
        #http://192.168.1.11/1?a=0&b=0
        currenturl = 'https://api.thingspeak.com/channels/708409/fields/1/last.json?api_key=OQON3Q83OX2G54HT'
        print currenturl
        response = urllib2.urlopen(currenturl)
        data = response.read()
        print data
        jsondata = json.loads(data)
        print jsondata.get('field1')
        response.close()
        speech = jsondata.get('field1')

        print("Response:")
        print(speech)
    else:
        speech = "default response"
   

    return {
        "speech": speech,
        "displayText": speech,
        #"data": {},
        # "contextOut": [],
        "source": "Sastra-App"
    }


if __name__ == '__main__':
    port = int(os.getenv('PORT', 5000))

    print "Starting app on port %d" % port

    app.run(debug=True, port=port, host='0.0.0.0')