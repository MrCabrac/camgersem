import { TestBed } from '@angular/core/testing';

import { BigqueryserviceService } from './bigqueryservice.service';

describe('BigqueryserviceService', () => {
  let service: BigqueryserviceService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(BigqueryserviceService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
