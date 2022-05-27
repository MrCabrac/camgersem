import { EventEmitter, Injectable, Output } from '@angular/core';
import { collection, collectionData, doc, docData, DocumentReference, Firestore, Timestamp } from '@angular/fire/firestore';
import { async } from '@firebase/util';
import { Observable } from 'rxjs';
import { Point2, Sensor } from '../interfaces/general';

/**
 * El dato que viene desde Firestore
 */
interface Point {
  time: Timestamp,
  humedad: number,
  iluminacion: number,
  temperatura: number
}

@Injectable({
  providedIn: 'root'
})
export class FirestoreService {
  sensor: Sensor = {name: "temperatura", values: []};
  @Output() temperature = new EventEmitter();
  constructor(
    private firestore: Firestore,
  ) {
  }

  getSensorValues = async () => {
    // console.log("getSensorValues");
    const temperaturaRef = doc(this.firestore, 'sensores/camgersem');
    docData(temperaturaRef).subscribe((response: any) => {
      // console.log("ahre");
      this.sensor.values = [];
      response.values.forEach((element: any) => {
        const vale = <Point>element;
        let hours = vale.time.toDate().getHours();
        let minutes = vale.time.toDate().getMinutes();
        let seconds = vale.time.toDate().getSeconds();
        const time = hours + ":" + minutes + ":" + seconds;
        const point2:Point2 = {time, humedad: vale.humedad, iluminacion: vale.iluminacion, temperatura: vale.temperatura};
        this.sensor.values.push(point2);
      });
      this.temperature.emit(this.sensor);
      // console.log(this.sensor);
    })
  }
}
