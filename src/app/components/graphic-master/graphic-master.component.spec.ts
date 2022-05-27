import { ComponentFixture, TestBed } from '@angular/core/testing';

import { GraphicMasterComponent } from './graphic-master.component';

describe('GraphicMasterComponent', () => {
  let component: GraphicMasterComponent;
  let fixture: ComponentFixture<GraphicMasterComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ GraphicMasterComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(GraphicMasterComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
