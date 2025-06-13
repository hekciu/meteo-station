import {
    useEffect,
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


type DataView = 'chart' | 'map';


interface EdgeDevice {
    name: string;
    location?: {
        latitude: number;
        longitude: number;
    };
    selected: boolean;
}


const getBasicAuth = () => {
    return 'Basic ' + base64.encode(API_USERNAME + ":" + API_PASSWORD);
}


export default function Measurements() {
    const [dataView, setDataView] = useState<DataView>('chart');
    const [devices, setDevices] = useState<EdgeDevice[]>([]);

    const onSelectedItemsChange = (selectedItems: { id: string, name: string }[]) => {
        const selectedItemsNames = selectedItems.map((i) => i.id);

        setDevices(devices.map((d) => selectedItemsNames.includes(d.name) ? {
            ...d,
            selected: true
        }: d));
    };


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
                selected: false
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
                <Map/> :
                <></>
            }
        </View>
    )
};

