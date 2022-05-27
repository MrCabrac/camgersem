export interface General {
}

export interface Sensor {
    name: string,
    values: Point2[]
}

/**
 * El dato para mostrarlo en la gráfica
 */
export interface Point2 {
    time: string,
    humedad: number,
    iluminacion: number,
    temperatura: number
}