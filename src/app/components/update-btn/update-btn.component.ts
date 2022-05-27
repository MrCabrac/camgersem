import { Component, OnInit } from '@angular/core';
import { BigqueryserviceService } from 'src/app/services/BigQueryService/bigqueryservice.service';
import { FirestoreService } from 'src/app/services/firestore.service';

@Component({
  selector: 'app-update-btn',
  templateUrl: './update-btn.component.html',
  styleUrls: ['./update-btn.component.css']
})
export class UpdateBtnComponent implements OnInit {
  clicked = false;

  constructor(
    private firestoreService: FirestoreService,
    private bqSrv: BigqueryserviceService
  ) { }

  ngOnInit(): void {
    // this.firestoreService.getSensorValues();
  }
  
  updateSensors(){
    this.firestoreService.getSensorValues();
  }

  bigQuery(){
    const q = "SELECT * FROM `camara-germinacion-semillas.datos_sensores.sensores` ORDER BY `time` DESC";
    // this.bqSrv.onQuery("camara-germinacion-semillas", q)
    //   .subscribe((result: any) => {
    //     console.log(result);
    //     if (!result)
    //       return;
    //     // const data = result['result'][0];
    //     // this.todolist = data;

    //   });
  }

}
