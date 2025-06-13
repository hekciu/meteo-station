import {
    useEffect,
    useRef,
useState
} from 'react';

import base64 from 'base-64'
import MultiSelect from 'react-native-multiple-select';

import {
View,
Button,
} from 'react-native';

import Map from './Map';
import { API_PASSWORD, API_URL, API_USERNAME } from '../variables';
import Charts from './Charts';


type DataView = 'chart' | 'map';


interface Pms5003Data {
    Pm10Standard: number,
    Pm25Standard: number,
    Pm100Standard: number,
    DeviceName: string,
    DeviceTimestamp: number,
};

export interface EdgeDevice {
    name: string;
    location?: {
        latitude: number;
        longitude: number;
    };
    selected: boolean;
    measurements: Pms5003Data[];
}


const getBasicAuth = () => {
    return 'Basic ' + base64.encode(API_USERNAME + ":" + API_PASSWORD);
}


const normalizeTimestamp = (timestamp: number) => Math.floor(timestamp / 1000)


export default function Measurements() {
    const [dataView, setDataView] = useState<DataView>('chart');
    const devicesRef = useRef<EdgeDevice[]>([]);
    const [devices, setDevices] = useState<EdgeDevice[]>(devicesRef.current);
    const [timestampFrom, setTimestampFrom] = useState(Date.now() - 1000*60*60*24*100);
    const [timestampTo, setTimestampTo] = useState(Date.now() + 5 * 1000);

    const onSelectedItemsChange = (selectedItems: { id: string, name: string }[]) => {
        /*
        const selectedItemsNames = selectedItems.map((i) => i.id);

        setDevices(devices.map((d) => selectedItemsNames.includes(d.name) ? {
            ...d,
            selected: true
        }: d));
        */
    };

    useEffect(() => {
        const interval = setInterval(() => {
            (async () => {
                const response = await fetch(`${API_URL}/pms5003?from=${normalizeTimestamp(timestampFrom)}&to=${normalizeTimestamp(timestampTo)}`, {
                    headers: [
                        ['Authorization', getBasicAuth()]
                    ],
                });

                const measurements = await response.json() as Pms5003Data[];

                const newDevices = devicesRef.current.map((d) => ({...d}));

                for (const device of newDevices) {
                    device.measurements = measurements
                        .filter((data) => data.DeviceName === device.name)
                        .sort((a, b) => a.DeviceTimestamp - b.DeviceTimestamp);
                }


                setDevices(newDevices);
                devicesRef.current = newDevices
            })()
        }, 10000);

        return () => clearInterval(interval);
    }, [timestampFrom, timestampTo]);


    useEffect(() => {
        (async () => {
            const response = await fetch(`${API_URL}/devices`, {
                headers: [
                    ['Authorization', getBasicAuth()]
                ]
            })

            const fetchedDevices = await response.json() as string[];

            const newDevices: EdgeDevice[] = fetchedDevices.map((name) => ({
                name: name,
                selected: false,
                measurements: []
            }))

            for (const deviceObj of newDevices) {
                const locationResponse = await fetch(`${API_URL}/technical/location?device_name=${deviceObj.name}`, {
                    headers: [
                        ['Authorization', getBasicAuth()]
                    ]
                });

                const location = await locationResponse.json() as {
                    DeviceName: string;
                    Latitude: number;
                    Longitude: number;
                };

                deviceObj.location = {
                    latitude: location.Latitude,
                    longitude: location.Longitude
                }
            }

            setDevices(newDevices);
            devicesRef.current = newDevices;
        })()
    }, [])

    return (
        <View style={{}}>
            <MultiSelect 
                selectText={'devices'}
                items={devices.map((device) => ({ id: device.name, name: device.name }))}
                selectedItems={devices.filter((d) => d.selected).map((d) => ({ id: d.name, name: d.name }))}
                onSelectedItemsChange={onSelectedItemsChange}
            />
            <Button title={dataView} onPress={() => {
                if (dataView === 'chart') {
                    setDataView('map');
                } else {
                    setDataView('chart');
                }
            }}/>
            {
                dataView === 'map' ?
                <Map
                    devices={devices}
                /> :
                <Charts/>
            }
        </View>
    )
};

