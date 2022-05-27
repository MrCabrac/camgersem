import { ComponentFixture, TestBed } from '@angular/core/testing';

import { UpdateBtnComponent } from './update-btn.component';

describe('UpdateBtnComponent', () => {
  let component: UpdateBtnComponent;
  let fixture: ComponentFixture<UpdateBtnComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ UpdateBtnComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(UpdateBtnComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
