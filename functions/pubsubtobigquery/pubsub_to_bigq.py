from google.cloud import bigquery
import base64
import json
import sys
import os

def pubsub_to_bigq(event, context):
    pubsub_message = base64.b64decode(event['data']).decode('utf-8')
    print(pubsub_message)
    to_bigquery(os.environ['dataset'],
                os.environ['table'], json.loads(pubsub_message))

def to_bigquery(dataset, table, document):
    bigquery_client = bigquery.Client()
    dataset_ref = bigquery_client.dataset(dataset)
    table_ref = dataset_ref.table(table)
    table = bigquery_client.get_table(table_ref)
    errors = bigquery_client.insert_rows(table, [document])
    if errors != []:
        print(errors, file=sys.stderr)
