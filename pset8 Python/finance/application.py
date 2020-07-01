import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # get data from db
    rows = db.execute("SELECT * FROM stocks WHERE user_id = :user",
        user=session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = :user",
        user=session["user_id"])[0]['cash']

    # variables for data storage
    ownedStocks = []
    totalValue = cash
    # iterate all rows store every stock info row, index onto i
    for i, row in enumerate(rows):
        # get stock real time data
        stock = lookup(row['symbol'])

        # make list of lists for ease of use in index.html
        list = [stock['symbol'], stock['name'], row['shares'], stock['price'], round(stock['price'] * row['shares'], 2)]
        ownedStocks.append(list)

        # get total value of your holdings
        totalValue += ownedStocks[i][4]

    return render_template("index.html", ownedStocks=ownedStocks, cash=round(cash, 2), totalValue=round(totalValue, 2))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # number of shares and stock data - name, price, symbol
        shares = int(request.form.get('shares'))
        stock = lookup(request.form.get('symbol'))

        # check if number is positive int
        if shares <= 0 or isinstance(shares, float):
            flash("Please input positive non decimal number of shares to buy")
            return redirect("/buy")

        # if we find stock by the ticker
        if stock:
            price = stock['price']
            cash = db.execute("SELECT cash FROM users WHERE id = :user",
                user=session["user_id"])[0]['cash']
            leftover = cash - price * float(shares)

            if leftover < 0:
                return render_template("buy.html", error=2)

            # check if user already owns stocks
            ownedStock = db.execute("SELECT shares FROM stocks WHERE user_id = :user AND symbol = :symbol",
                user=session["user_id"], symbol=stock['symbol'])

            # insert new row into stocks table
            if not ownedStock:
                db.execute("INSERT INTO stocks(user_id, symbol, shares) VALUES (:user, :symbol, :shares)",
                    user=session["user_id"], symbol=stock['symbol'], shares=shares)
            # update row in stock table
            else:
                shares += ownedStock[0]['shares']
                db.execute("UPDATE stocks SET shares = :shares WHERE user_id = :user AND symbol = :symbol",
                    user=session["user_id"], symbol=stock['symbol'], shares=shares)

            # update cash
            db.execute("UPDATE users SET cash = :cash WHERE id = :user",
                cash=leftover, user=session["user_id"])
            # Update history table
            db.execute("INSERT INTO history(user_id, symbol, shares, value) VALUES (:user, :symbol, :shares, :value)",
                user=session["user_id"], symbol=stock['symbol'], shares=shares, value=round(price * float(shares), 2))

            # Redirect user with a success message
            flash("Assets successfully acquired")
            return redirect("/")

        # if there is no stock by provided ticker
        else:
            return render_template("buy.html", error=1)
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # get trade history from db
    rows = db.execute("SELECT * FROM history WHERE user_id = :user ORDER BY date DESC",
        user=session["user_id"])

    # list to story history data
    history = []
    for row in rows:
        # get stock real time data
        stock = lookup(row['symbol'])

        # list with all the info about individual stock
        list = [stock['symbol'], stock['name'], row['shares'], row['value'], row['date']]
        # append data about individual stock to list of all the stocks
        history.append(list)

    # redirect user with data to view
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # user reached route with post (form was submited)
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        return render_template("quote.html", stock=stock)
    # User reached route by clicking a link or redirect
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # clear user_id
    session.clear()
    regPass = request.form.get("pass")
    regUsername = request.form.get("username")
    # user reached route with post (form was submited)
    if request.method == "POST":

        # error handlers - ensure passwords match and are correct format, username not taken
        if regPass != request.form.get("confirm-pass"):
            return apology("The passwords don't match", 403)
        # double password validation, because js can be turned off
        elif len(regPass) > 5 and regPass.isalnum() == False:
            return apology("Password is not alphanumeric", 403)
        elif db.execute("SELECT * FROM users WHERE username = :username",
            username=regUsername):
            return apology("Username already taken", 403)

        # insert new user into db then execute function returns primary key(id) of the inserted user
        newUserId = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
            username=regUsername, hash=generate_password_hash(regPass))

        # remember logged in user by id
        session["user_id"] = newUserId

        # redirect to homepage
        return redirect("/")
    # user reached route by pushing register or url.../register
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # set variables
        shares=int(request.form.get("shares"))
        symbol=request.form.get("symbol")
        price=lookup(symbol)["price"]
        value=round(price*float(shares))

        # check if number is positive int
        if shares <= 0 or isinstance(shares, float):
            flash("Please input positive non decimal number of shares to sell")
            return redirect("/sell")

        # get data on shares after sell
        sharesBefore = db.execute("SELECT shares FROM stocks WHERE user_id = :user AND symbol = :symbol",
            user=session["user_id"], symbol=symbol)[0]['shares']
        sharesAfter = sharesBefore - shares

        # delete if sold everything
        if sharesAfter == 0:
            db.execute("DELETE FROM stocks WHERE user_id = :user AND symbol = :symbol",
                user=session["user_id"], symbol=symbol)
        # not enough shares - stop and return notification
        elif sharesAfter < 0:
            flash("Can not sell more stocks than you own")
            return redirect("/sell")
        # if all good = update db
        else:
            db.execute("UPDATE stocks SET shares = :shares WHERE user_id = :user AND symbol = :symbol",
                shares=sharesAfter, user=session["user_id"], symbol=symbol)

        # update cash
        cash = db.execute("SELECT cash FROM users WHERE id = :user",
            user=session["user_id"])[0]['cash']
        cashAfter = cash + price * float(shares)
        db.execute("UPDATE users SET cash = :cash WHERE id = :user",
            cash=cashAfter, user=session["user_id"])

        # update history
        db.execute("INSERT INTO history (user_id, symbol, shares, value) VALUES (:user, :symbol, :shares, :value)",
            user=session["user_id"], symbol=symbol, shares=-shares, value=value)

        # Redirect + success message
        flash("Assets successfully sold")
        return redirect("/")
    # User reached route by clicking a link or redirect
    else:
        # get owned stocks data
        rows = db.execute("SELECT symbol, shares FROM stocks WHERE user_id = :user",
            user=session["user_id"])

        # list of lists to store symbol[0] and shares[1]
        stocks = []
        for row in rows:
            stock = lookup(row['symbol'])
            stocks.append(list((stock['symbol'], row['shares'])))

        # render page and return date for view
        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
