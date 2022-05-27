import { Component, OnInit, ViewChild } from '@angular/core';
import { ChartConfiguration, ChartDataset, ChartEvent, ChartType } from 'chart.js';
import { BaseChartDirective } from 'ng2-charts';
import { Subscription } from 'rxjs';
import { Point2, Sensor } from 'src/app/interfaces/general';
import { FirestoreService } from 'src/app/services/firestore.service';

@Component({
  selector: 'app-graphic-master',
  templateUrl: './graphic-master.component.html',
  styleUrls: ['./graphic-master.component.css']
})
export class GraphicMasterComponent implements OnInit {

  // Variables
  xlabel: string[] = [];
  temperature: number[] = [];
  humedad: number[] = [];
  iluminacion: number[] = [];
  // Subscriptions
  private sensorsSubscription: Subscription = new Subscription;

  public lineChartData: ChartConfiguration['data'] = {
    datasets: [
      {
        data: [13, 14, 15, 16, 17, 16, 15, 16, 17, 18, 19, 20, 21, 20, 19.5, 19.8, 19.5, 20, 34],
        label: 'Temperatura',
        backgroundColor: 'rgba(255,0,0,0.3)',
        borderColor: 'rgba(255,0,0,1)',
        pointBackgroundColor: 'rgba(184, 44, 44,1)',
        pointBorderColor: '#fff',
        pointHoverBackgroundColor: '#fff',
        pointHoverBorderColor: 'rgba(184, 44, 44,0.8)',
        fill: 'origin',
      },
      {
        data: [70, 65, 60, 61, 55, 52, 56, 54, 52, 52, 51, 49, 48, 49, 45, 43, 48, 45],
        label: 'Humedad',
        backgroundColor: 'rgba(0,0,255,0.2)',
        borderColor: 'rgba(0,0,255,1)',
        pointBackgroundColor: 'rgba(25,155,191,1)',
        pointBorderColor: '#fff',
        pointHoverBackgroundColor: '#fff',
        pointHoverBorderColor: 'rgba(25,155,191,1)',
        fill: 'origin',
      },
      {
        data: [40, 40, 40, 45, 40, 35, 38, 35, 36, 42, 41, 89, 90, 85, 86, 45, 32, 25],
        label: 'Iluminacion',
        yAxisID: 'y-axis-1',
        backgroundColor: 'rgba(0,255,0,0.1)',
        borderColor: 'green',
        pointBackgroundColor: 'rgba(0,255,0,1)',
        pointBorderColor: '#fff',
        pointHoverBackgroundColor: '#fff',
        pointHoverBorderColor: 'rgba(148,159,177,0.8)',
        fill: 'origin',
      }
    ],
    labels: ['00:00', '00:01', '00:02', '00:03', '00:04', '00:05', '00:06', '00:07', '00:08', '00:09', '00:10', '00:11', '00:12', '00:13', '00:14', '00:15', '00:16', '00:17']
  };

  public lineChartOptions: ChartConfiguration['options'] = {
    elements: {
      line: {
        tension: 0.5
      }
    },
    responsive: true,
    scales: {
      // We use this empty structure as a placeholder for dynamic theming.
      x: {},
      'y-axis-0':
      {
        position: 'left',
      },
      'y-axis-1': {
        position: 'right',
        grid: {
          color: 'rgba(255,0,0,0.3)',
        },
        ticks: {
          color: 'red'
        }
      }
    },
    plugins: {
      legend: { display: true },
    }
  }

  public lineChartType: ChartType = 'line';
  @ViewChild(BaseChartDirective) chart?: BaseChartDirective;
  constructor(
    private firestoreService: FirestoreService
  ) {
    this.subscribeFirestoreService();
  }

  ngOnInit(): void {
  }

  public chartHovered({ event, active }: { event?: ChartEvent, active?: {}[] }): void {
    // console.log(event, active);
  }

  public chartClicked({ event, active }: { event?: ChartEvent, active?: {}[] }): void {
    // console.log(event, active);
  }

  subscribeFirestoreService(){
    return this.firestoreService.temperature.subscribe(response=>{
      // console.log(response);
      this.xlabel = [];
      this.temperature = [];
      this.iluminacion = [];
      this.humedad = [];
      response.values.forEach((element: Point2) => {
        this.xlabel.push(element.time);
        this.temperature.push(element.temperatura);
        this.humedad.push(element.humedad);
        this.iluminacion.push(element.iluminacion);
      });
      this.lineChartData.labels = this.xlabel.slice(-50,-1);
      this.lineChartData.datasets[0].data = this.temperature.slice(-50,-1);
      this.lineChartData.datasets[1].data = this.humedad.slice(-50,-1);
      this.lineChartData.datasets[2].data = this.iluminacion.slice(-50,-1);
      // console.log(this.lineChartData.labels);
      this.chart?.update();
    })
  }

}
