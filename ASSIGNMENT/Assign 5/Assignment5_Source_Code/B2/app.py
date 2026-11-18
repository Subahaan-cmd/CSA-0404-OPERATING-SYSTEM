# app.py - sample result portal (Flask)
import os
from flask import Flask, jsonify

app = Flask(__name__)
RESULTS = {"2501001": {"name": "Asha", "cgpa": 8.7}, "2501002": {"name": "Ravi", "cgpa": 7.9}}

@app.route("/")
def index():
    return "University Result Portal - enter your register number at /result/<regno>\n"

@app.route("/health")
def health():
    return jsonify(status="ok")          # used by the readiness probe

@app.route("/result/<regno>")
def result(regno):
    r = RESULTS.get(regno)
    return (jsonify(r), 200) if r else (jsonify(error="not found"), 404)
