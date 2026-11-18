# Container image for the campus result-portal web service (a small Flask app).
FROM python:3.12-slim
WORKDIR /app
# Install the dependencies first so that this layer is cached between builds.
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY . .
RUN useradd --create-home --shell /usr/sbin/nologin portal && chown -R portal:portal /app
USER portal
EXPOSE 8000
CMD ["gunicorn", "--bind", "0.0.0.0:8000", "app:app"]
