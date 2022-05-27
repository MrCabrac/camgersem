'use strict';
import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
// import { BigQuery } from '@google-cloud/bigquery';
// const { BigQuery } = require('@google-cloud/bigquery');

@Injectable({
  providedIn: 'root'
})
export class BigqueryserviceService {

  constructor(
  ) { }

  onQuery(datasetName: string, q: string) {
    // const bigquery = new BigQuery();
    // const QUERY_URL: string = `YOUR_CLOUD_FUNCTIONS_URL`;
    // const postParams = {
    //   datasetName: datasetName,
    //   query: q
    // };
    // return this.httpClient.post(QUERY_URL, postParams);
    // return "a";
  }
}
